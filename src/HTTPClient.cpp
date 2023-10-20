/*************************************************************************************************************
*
* Title			    : Free-ESPatHome
* Description:      : Library that implements the Busch-Jeager / ABB Free@Home API for ESP8266 and ESP32.
* Version		    : v 0.2
* Last updated      : 2023.10.20
* Target		    : ESP32, ESP8266, ESP8285
* Author            : Roeland Kluit
* Web               : https://github.com/roelandkluit/Free-ESPatHome
* License           : GPL-3.0 license
*
**************************************************************************************************************/
#include "HTTPClient.h"
#include <WiFiClientSecure.h>

HTTPClient::HTTPClient(const bool& Secure)
{
    if (Secure)
    {
        WiFiClientSecure* wcs = new WiFiClientSecure();
        wcs->setInsecure();  // Disable certificate verification
        this->client = wcs;
    }
    else
    {    
        this->client = new WiFiClient();
    }
    ReturnBody.reserve(MAXBODYSIZE);
}

bool HTTPClient::Connect(const String& RemoteHost, const unsigned int& port)
{    	
    if(this->state > HTTPCLIENT_STATE::HTTPCLIENT_STATE_CLOSED)
    {
        DEBUG_P(F("Connection in progess: "));
        DEBUG_P(this->state);
        return false;
    }

    ClearVariables();

	if (!client->connect(RemoteHost.c_str(), port))
    {
        DEBUG_PL(F("Connection failed"));
		return false;
    }

    this->state = HTTPCLIENT_STATE::HTTPCLIENT_STATE_CONNECTED;
    return true;
}

bool HTTPClient::ReadResult(uint16_t* resultcode)
{
    //Todo Add timeout
    if(this->state == HTTPCLIENT_STATE::HTTPCLIENT_STATE_REQUESTED)
    {
        if(sessionOK())
        {
            if(client->available())
            {
                String line = client->readStringUntil('\n');
                //DEBUG_PL(line);
                if (line.indexOf(F("HTTP/")) != -1)
                {
                    String status = line.substring(9, 12);
                    LastResult = status.toInt();
                    *resultcode = LastResult;
                    this->state = HTTPCLIENT_STATE::HTTPCLIENT_STATE_HEADERS;
                    return true;
                }
                else
                {
                    //Invalid resonse 
                    *resultcode = 2;
                }
            }
            else
            {
                *resultcode = 0;
                return false; //Data not yet recieved
            }
        }
        else
        {
            this->client->stop();
            this->state = HTTPCLIENT_STATE::HTTPCLIENT_STATE_FAILED;
            return false;
        }
    }
    else
    {
        //DEBUG_PL(F("Wrong state"));
    }
    return false;
}

HTTPCLIENT_STATE HTTPClient::GetState()
{
    return this->state;
}

bool HTTPClient::sessionOK()
{
    if(this->state > HTTPCLIENT_STATE::HTTPCLIENT_STATE_CLOSED)
    {
        if(client->connected())
            return true;
        if(client->available())
            return true;
    }
    return false;
}

bool HTTPClient::ReadHeaders(String& Key, String& Value)
{
    if(this->state == HTTPCLIENT_STATE::HTTPCLIENT_STATE_HEADERS)
    {
        if(sessionOK())
        {
            if(client->available())
            {
                String line = client->readStringUntil('\n');
                if(line.length() <= 1)
                {
                    //DEBUG_PL(F("DataStart"));
                    this->state = HTTPCLIENT_STATE::HTTPCLIENT_STATE_DATA;
                }
                else
                {
                    //Todo check max lenght of headers??
			        int col = line.indexOf(":");
                    if(col > 0)
                    {
			            Key = line.substring(0, col);
			            Key.toLowerCase();  // Make all headers lowercase for case-insensitve comparison
			            Value = line.substring(col + 2, line.length() - 1);
                        ReturnHeaderKeys += Key + "\n";
                        ReturnHeaderValues += Value + "\n";                        
                        return true;                
                    }
                }
            }
        }
        else
        {
            this->client->stop();
            this->state = HTTPCLIENT_STATE::HTTPCLIENT_STATE_FAILED;
        }
    }
    return false;
}

String HTTPClient::GetBody()
{
    return this->ReturnBody;
}

bool HTTPClient::ReadPayload()
{
    if(this->state == HTTPCLIENT_STATE::HTTPCLIENT_STATE_DATA)
    {
        if(sessionOK())
        {
            if(client->available())
            {
                char t = client->read(); 
                //DEBUG_P(t);
                if (this->ReturnBody.length() + 1 > MAXBODYSIZE)
                {
                    DEBUG_PL(F("NotAppending ReturnBody, OutOfMem"));
                }                    
                else
                {
                    this->ReturnBody += t;
                }
                return true;
            }
        }
        else
        {
            this->client->stop();
            Serial.println("Stopping");
            this->state = HTTPCLIENT_STATE::HTTPCLIENT_STATE_CLOSED;
        }
    }
    return false;
}

bool HTTPClient::FullRequest(const String& Host, const String& URL, const String& PostData)
{
    return false;
}

void HTTPClient::ClearVariables()
{
    AdditionalHeaders = "";
    ReturnHeaderKeys = "";
    ReturnHeaderValues = "";
    LastResult = 0;
    ReturnBody = "";
}

void HTTPClient::abort()
{
    if(this->state >= HTTPCLIENT_STATE::HTTPCLIENT_STATE_CONNECTED)
    {
        Serial.println("AbortConn");
        this->client->stop();
        this->state = HTTPCLIENT_STATE::HTTPCLIENT_STATE_CLOSED;     
        ClearVariables();
    }
}

bool HTTPClient::Request(const String& HTTPCommand, const String& URL, const String& PostData)
{
    if(this->state == HTTPCLIENT_STATE::HTTPCLIENT_STATE_CONNECTED)
    {
    	String HTTPrequestPacket = HTTPCommand + " " + URL + F(" HTTP/1.1\r\nConnection: Close\r\n");

        if(PostData.length() != 0)
        {
            String slen = String(PostData.length());
            HTTPrequestPacket += String(F("Content-Length: ")) + slen + "\r\n";
        }

        HTTPrequestPacket += AdditionalHeaders;

        if(PostData.length() != 0)
            HTTPrequestPacket += "\r\n" + PostData;

        //DEBUG_PL(HTTPrequestPacket);
        size_t ret = client->write(HTTPrequestPacket.c_str());        
        if(ret == HTTPrequestPacket.length())
        {
            state = HTTPCLIENT_STATE::HTTPCLIENT_STATE_REQUESTED;
            return true;
        }
        else
        {
            client->stop();
        }
    }    
    return false;
}

void HTTPClient::AddRequestHeader(const String &Key, const String &Value)
{
    this->AdditionalHeaders += Key + ": " + Value + "\r\n";
}

void HTTPClient::ClearRequestHeaders()
{
    this->AdditionalHeaders = "";
}


HTTPClient::~HTTPClient()
{
    if(this->client != NULL)
    {
        if(this->client->connected())
        {
            this->client->stop();            
        }
        delete this->client;
        this->client  = NULL;
    }
}
