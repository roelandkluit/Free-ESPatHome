/*************************************************************************************************************
*
* Title			    : Free-ESPatHome
* Description:      : Library that implements the Busch-Jeager / ABB Free@Home API for ESP8266 and ESP32.
* Version		    : v 0.10
* Last updated      : 2023.12.13
* Target		    : ESP32, ESP8266, ESP8285
* Author            : Roeland Kluit
* Web               : https://github.com/roelandkluit/Free-ESPatHome
* License           : GPL-3.0 license
*
**************************************************************************************************************/
#include "FahHTTPClient.h"

String FahHTTPClient::LastURIRequested()
{
	return Async_URI;
}

String FahHTTPClient::LastRequestMethod()
{
	return Async_Method;
}

HTTPREQUEST_STATUS FahHTTPClient::GetAsyncStatus()
{
	return AsyncStatus;
}

void FahHTTPClient::ReleaseAsync()
{
	this->abort();
	Async_Method = "";
	Async_URI = "";
	Async_PostData = "";
	AsyncStatus = HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_NONE;	
}

FahHTTPClient::FahHTTPClient(FahSysAPInfo* sysApInfo) :HTTPClient(sysApInfo->secure)
{
	this->SysAp = sysApInfo;
}

bool FahHTTPClient::ConnectToSysAp()
{
	if (this->GetState() <= HTTPCLIENT_STATE::HTTPCLIENT_STATE_CLOSED)
	{
		if (!this->Connect(this->SysAp->Hostname.c_str(), this->SysAp->port))
		{
			this->abort();
			return false;
		}
		else
		{
			return true;
		}
	}
	return false;
}

bool FahHTTPClient::HTTPRequestAsync(const String& Method, const String& URI, const String& PostData)
{
	if (AsyncStatus != HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_PENDING)
	{
		if (!ConnectToSysAp())
		{
			DEBUG_PL(F("Failed to Connect"));
			return false;
		}
		Async_Method = Method;
		Async_URI = URI;
		Async_PostData = PostData;
		AsyncStatus = HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_PENDING;
		return true;
	}
	return false;
}

void FahHTTPClient::ProcessAsync()
{
	HTTPREQUEST_STATUS reqStatus;

	switch (this->GetState())
	{
		case HTTPCLIENT_STATE::HTTPCLIENT_STATE_INITIAL:
		case HTTPCLIENT_STATE::HTTPCLIENT_STATE_CLOSED:
			if (AsyncStatus == HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_PENDING)
			{
				//Todo Check payload (getBody) size?
				AsyncStatus = HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_SUCCESS;
			}
			return;

		case HTTPCLIENT_STATE::HTTPCLIENT_STATE_FAILED:
			AsyncStatus = HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_FAILED;

		case HTTPCLIENT_STATE::HTTPCLIENT_STATE_CONNECTED:
			if (!PutHTTPRequest(Async_Method, Async_URI, Async_PostData))
			{
				DEBUG_PL(F("ASYNC_HTTP Failed"));
			}
			return;

		case HTTPCLIENT_STATE::HTTPCLIENT_STATE_REQUESTED:
			reqStatus = GetHTTPRequestResult();
			if (reqStatus ==  HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_PENDING)
			{
				return;
			}
			else if (reqStatus != HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_SUCCESS)
			{
				DEBUG_PL(F("ASYNC_HTTP Response Failed"));
			}
			return;

		case HTTPCLIENT_STATE::HTTPCLIENT_STATE_HEADERS:
			reqStatus = ProcessHTTPHeaders();
			if (reqStatus == HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_PENDING)
			{
				return;
			}
			else if(reqStatus != HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_SUCCESS)
			{
				DEBUG_PL(F("ASYNC_HTTP Headers Failed"));
			}
			return;

		case HTTPCLIENT_STATE::HTTPCLIENT_STATE_DATA:
			reqStatus = ProcessHTTPBody();

			if (reqStatus == HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_PENDING)
			{
				return;
			}
			else if (reqStatus != HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_SUCCESS)
			{
				DEBUG_PL(F("ASYNC_HTTP Data Failed"));
			}
			else
			{
				AsyncStatus = HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_SUCCESS;				
				DEBUG_PL("SuccesAsyncA");				
			}
			return;

		default:
			return;
	}
}


bool FahHTTPClient::HTTPRequest(const String& Method, const String& URI, const String& PostData)
{
	if (!ConnectToSysAp())
	{
		DEBUG_PL(F("Failed to Connect"));
		return false;
	}

	if (!PutHTTPRequest(Method, URI, PostData))
	{
		DEBUG_PL(F("Put Failed"));
		return false;
	}

	HTTPREQUEST_STATUS httpResultStatus = HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_PENDING;
	while (httpResultStatus == HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_PENDING)
	{
		delay(25);
		httpResultStatus = GetHTTPRequestResult();
	}

	if (httpResultStatus != HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_SUCCESS)
	{
		DEBUG_PL(F("Put response error"));
		return false;
	}

	HTTPREQUEST_STATUS httpHeaderStatus = HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_PENDING;
	while (httpHeaderStatus == HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_PENDING)
	{
		delay(25);
		httpHeaderStatus = ProcessHTTPHeaders();
	}

	if (httpHeaderStatus != HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_SUCCESS)
	{
		DEBUG_PL(F("HTTP header response error"));
		return false;
	}

	HTTPREQUEST_STATUS httpBodyStatus = HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_PENDING;
	while (httpBodyStatus == HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_PENDING)
	{
		delay(25);
		httpBodyStatus = ProcessHTTPBody();
	}

	if (httpBodyStatus != HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_SUCCESS)
	{
		DEBUG_PL(F("HTTP body response error"));
		return false;
	}
	return true;
}

bool FahHTTPClient::PutHTTPRequest(const String& Method, const String& URI, const String& PostData)
{
	if (this->GetState() == HTTPCLIENT_STATE::HTTPCLIENT_STATE_CONNECTED)
	{
		String appjson = String(F("application/json"));
		this->AddRequestHeader(String(F("Content-Type")), appjson);
		this->AddRequestHeader(String(F("Accept")), appjson);
		this->AddRequestHeader(String(F("Host")), String(F("sysap")));
		if (this->SysAp->authorizationHeader.length() != 0)
		{
			this->AddRequestHeader(String(F("Authorization")), this->SysAp->authorizationHeader);
		}

		if (!this->Request(Method, URI, PostData))
		{
			this->abort();
			return false;
		}
		else
		{
			return true;
		}
	}
	return false;
}

HTTPREQUEST_STATUS FahHTTPClient::ProcessHTTPHeaders()
{
	if (this->GetState() == HTTPCLIENT_STATE::HTTPCLIENT_STATE_HEADERS)
	{
		String Key;
		String Value;
		if (this->ReadHeaders(Key, Value))
		{
			//DEBUG_P("hdr: "); DEBUG_P(Key);	DEBUG_P("-->"); DEBUG_PL(Value);
			return HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_PENDING;
		}
		else
		{
			return HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_SUCCESS;
		}
	}
	return HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_FAILED;
}

HTTPREQUEST_STATUS FahHTTPClient::ProcessHTTPBody()
{
	if (this->GetState() == HTTPCLIENT_STATE::HTTPCLIENT_STATE_DATA)
	{
		this->ReadPayload();
		return HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_PENDING;
	}

	if (this->GetState() == HTTPCLIENT_STATE::HTTPCLIENT_STATE_CLOSED)
	{
		return HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_SUCCESS;
	}
	return HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_FAILED;
}

HTTPREQUEST_STATUS FahHTTPClient::GetHTTPRequestResult()
{
	if (this->GetState() == HTTPCLIENT_STATE::HTTPCLIENT_STATE_REQUESTED)
	{
		uint16_t resultcode = 0xFFFF;
		if (this->ReadResult(&resultcode))
		{
			if (resultcode != 200)
			{
				DEBUG_P(F("DeviceFailed: HTTP_STATUS_"));
				DEBUG_PL(resultcode);
				this->abort();
				return HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_FAILED;
			}
			else
			{
				return HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_SUCCESS;
			}
		}
		else
		{
			if(resultcode == 0)
				return HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_PENDING;
			else
				return HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_FAILED;
		}
	}
	return HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_FAILED;
}

FahHTTPClient::~FahHTTPClient()
{
	this->SysAp = NULL;
}
