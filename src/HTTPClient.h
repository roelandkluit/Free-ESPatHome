/*************************************************************************************************************
*
* Title			    : Free-ESPatHome
* Description:      : Library that implements the Busch-Jeager / ABB Free@Home API for ESP8266 and ESP32.
* Version		    : v 0.4
* Last updated      : 2023.10.29
* Target		    : ESP32, ESP8266, ESP8285
* Author            : Roeland Kluit
* Web               : https://github.com/roelandkluit/Free-ESPatHome
* License           : GPL-3.0 license
*
**************************************************************************************************************/
#pragma once
#include <WiFiClient.h>
#include "BuildConfig.h"

#define MAXBODYSIZE 2048

namespace HTTPCLIENT_STATES
{
	enum HTTPCLIENT_STATE :uint8_t
	{
		HTTPCLIENT_STATE_INITIAL = 0,
        HTTPCLIENT_STATE_FAILED = 1,
        HTTPCLIENT_STATE_CLOSED = 10,
        HTTPCLIENT_STATE_CONNECTED = 20,
		HTTPCLIENT_STATE_REQUESTED = 30,
        HTTPCLIENT_STATE_HEADERS = 40,
        HTTPCLIENT_STATE_DATA = 50,        
	};
}
typedef HTTPCLIENT_STATES::HTTPCLIENT_STATE HTTPCLIENT_STATE;

class HTTPClient
{
private:
    WiFiClient* client = NULL;
    HTTPCLIENT_STATE state = HTTPCLIENT_STATE::HTTPCLIENT_STATE_INITIAL;
    String AdditionalHeaders = "";
    String ReturnHeaderKeys = "";
    String ReturnHeaderValues = "";
    String ReturnBody = "";
    uint16_t LastResult = 0;
    void ClearVariables();
    /* data */
public:
    HTTPClient(const bool &secure);
    bool Connect(const String &RemoteHost, const unsigned int &port);    
    ~HTTPClient();
    bool Request(const String &HTTPCommand, const String &URL, const String &PostData);
    bool ReadResult(uint16_t *resultcode);
    HTTPCLIENT_STATE GetState();
    bool sessionOK();
    bool ReadHeaders(String &Key, String& Value);
    String GetBody();
    bool ReadPayload();
    void abort();
    bool FullRequest(const String &Host, const String &URL, const String &PostData);
    void AddRequestHeader(const String &Key, const String &Value);
    void ClearRequestHeaders();
};