/*************************************************************************************************************
*
* Title			    : Free-ESPatHome
* Description:      : Library that implements the Busch-Jeager / ABB Free@Home API for ESP8266 and ESP32.
* Version		    : v 0.6
* Last updated      : 2023.11.06
* Target		    : ESP32, ESP8266, ESP8285
* Author            : Roeland Kluit
* Web               : https://github.com/roelandkluit/Free-ESPatHome
* License           : GPL-3.0 license
*
**************************************************************************************************************/
#include "FahESPDevice.h"
#include "FreeAtHomeESPapi.h"

bool FahESPDevice::EnqueDataPoint(const bool& GetValue, const String Channel, const String DataPoint, const String Value)
{
	String reqType = String(F("GET"));
	if (!GetValue)
		reqType = String(F("PUT"));

	String sDeviceID = FreeAtHomeESPapi::U64toString(FahDevice);
	String Entry = reqType + ":" + sDeviceID + "." + Channel + "." + DataPoint + ":" + Value;
	//DEBUG_PL(Entry.c_str());
	return EnqueSetDataPoint(Entry);
}

bool FahESPDevice::EnqueSetDataPoint(const String Channel, const String DataPoint, const String Value)
{
	return EnqueDataPoint(false, Channel, DataPoint, Value);
}

bool FahESPDevice::EnqueGetDataPoint(const String Channel, const String DataPoint)
{
	return EnqueDataPoint(true, Channel, DataPoint, "");
}

bool FahESPDevice::EnqueSetDataPoint(const String Entry)
{
	if (PendingDataPointsCount == MAX_PENDING_DATAPOINTS)
		return false;

	PendingDataPointsCount++;

	String* newStrArray = new String[PendingDataPointsCount];
	for (uint8_t i = 0; i < PendingDataPointsCount - 1; i++)
	{
		newStrArray[i] = PendingDataPoints[i]; //Copy all items in the new array
	}
	newStrArray[PendingDataPointsCount - 1] = Entry; //Append new item to the end

	if (PendingDataPoints != NULL)
		delete[] PendingDataPoints;

	PendingDataPoints = newStrArray;
	return true;
}

String FahESPDevice::DequeDataPoint()
{
	if (PendingDataPointsCount == 0)
		return "";

	PendingDataPointsCount--;

	String* newStrArray = new String[PendingDataPointsCount];
	String returnStr = "" + PendingDataPoints[0]; //Get first item from list

	for (uint8_t i = 0; i < PendingDataPointsCount; i++)
	{
		newStrArray[i] = PendingDataPoints[i + 1]; //Move all items 1 up in the new array
	}	

	delete[] PendingDataPoints;
	PendingDataPoints = newStrArray;
	return returnStr;
}

void FahESPDevice::NotifyFahDataPoint(const String& strChannel, const String& strDataPoint, const String& strValue, const bool& isSceneOrGetValue)
{
}

uint64_t FahESPDevice::GetFahDeviceID()
{
	return FahDevice;
}

FahESPDevice::FahESPDevice(const String& FahDeviceType, const uint64_t& FahAbbID, const String& SerialNr, const uint16_t& timeout, FreeAtHomeESPapi* fahParent, FahSysAPInfo* SysApInfo)
{
	this->httpclt = new FahHTTPClient(SysApInfo);
	this->SysApApi = fahParent;
	this->FahDevice = FahAbbID;
	this->TimeOut = timeout;
	this->FahDeviceType = FahDeviceType;
	this->SerialNr = SerialNr;
	this->LastWaitInterval = millis();
	this->WaitTimeMs = (timeout * 3000) / 4;
}

FahESPDevice::~FahESPDevice()
{
	if (httpclt != NULL)
	{
		delete httpclt;
		httpclt = NULL;
	}
}

String FahESPDevice::GetJsonDeviceValueFromResponse(const String &Response)
{
	DynamicJsonDocument fahJsonMsg(2000);
	DeserializationError error = deserializeJson(fahJsonMsg, Response);

	if (error)
	{
		// Test if parsing succeeds.
		//DEBUG_P(F("deserializeJson() failed: ")); DEBUG_PL(error.f_str());
		//DEBUG_PL(Response);
		return "";
	}
	else
	{
		if (fahJsonMsg.containsKey(FreeAtHomeESPapi::KEY_ROOT))
		{
			JsonObject root = fahJsonMsg[FreeAtHomeESPapi::KEY_ROOT].as<JsonObject>();
			if (root.containsKey(FreeAtHomeESPapi::KEY_VALUES))
			{
				JsonArray values = root[FreeAtHomeESPapi::KEY_VALUES].as<JsonArray>();
				if (values.size() == 1)
				{
					String s = values[0].as<String>();
					//DEBUG_P(s.c_str());
					return s;
				}
			}
		}
	}
	return "";
}

bool FahESPDevice::GetDataPointAndChannelFromURL(const String& URL, String& channel, String& datapoint)
{
	uint16_t end_pos = 0;
	uint16_t start_pos = 0;

	String sDeviceID = FreeAtHomeESPapi::U64toString(FahDevice) + ".";
	start_pos = URL.indexOf(sDeviceID);

	if (start_pos > 0)
	{
		start_pos = start_pos + sDeviceID.length(); //For the starting '[devicename].' before the channel
		end_pos = URL.indexOf('.', start_pos + 1);
		if (end_pos > 0)
		{
			channel = URL.substring(start_pos, end_pos);
			datapoint = URL.substring(end_pos + 1);
			return true;
		}
	}
	return false;
}

void FahESPDevice::processBase()
{
	if (httpclt->GetAsyncStatus() == HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_PENDING)
	{
		if ((millis() - httpclt->GetSessionStartMillis()) > HTTP_SESSION_TIMEOUT_MS)
		{
			httpclt->ReleaseAsync();
			lastSendGap = 200;
		}
		else
		{
			httpclt->ProcessAsync();
		}
	}
	else if (httpclt->GetAsyncStatus() == HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_FAILED)
	{
		//DEBUG_PL(httpclt->)
		httpclt->ReleaseAsync();
		lastSendGap = 200;
	}
	else if (httpclt->GetAsyncStatus() == HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_SUCCESS)
	{
		if (httpclt->LastRequestMethod() == String(F("GET")))
		{
			if (httpclt->GetResponseHeaderByKey(String(F("content-type"))) == String(F("application/json")))
			{
				//DEBUG_P("Process: ");
				String Channel = "";
				String DataPoint = "";
				if (GetDataPointAndChannelFromURL(httpclt->LastURIRequested(), Channel, DataPoint))
				{
					String returndata = httpclt->GetBody();
					String value = GetJsonDeviceValueFromResponse(returndata);
					if (value.length() > 0)
					{
						//DEBUG_F("CH: %s, DP: %s, Val: %s\r\n", Channel.c_str(), DataPoint.c_str(), value.c_str());
						NotifyFahDataPoint(Channel, DataPoint, value, true);
					}
				}
			}
		}
		httpclt->ReleaseAsync();
		lastSendGap = 10;
	}
	else if ((millis() - this->LastWaitInterval) > this->WaitTimeMs)
	{
		if (lastSendGap == 0)
		{
			//DEBUG_P(F("Update: ")); DEBUG_PL((millis() - this->LastWaitInterval));

			String URI = FreeAtHomeESPapi::ConstructDeviceRegistrationURI(SerialNr);
			String HTTPPostData = FreeAtHomeESPapi::ConstructDeviceRegistrationBody(this->FahDeviceType, "", this->TimeOut);
			if (httpclt->HTTPRequestAsync(String(F("PUT")), URI, HTTPPostData))
			{
				//ASync started, reset counter
				this->LastWaitInterval = millis();
			}
		}
	}
	else if (PendingDataPointsCount > 0)
	{
		if (lastSendGap == 0)
		{
			String strDataPoint = DequeDataPoint();
			String strDataPointRequestType = "";
			String strDataPointPart = "";
			String strDataPointValue = "";

			uint8_t token_idx = 0;
			if (FreeAtHomeESPapi::GetStringToken(strDataPoint, strDataPointRequestType, token_idx, ':'))
			{
				token_idx++;
				if (FreeAtHomeESPapi::GetStringToken(strDataPoint, strDataPointPart, token_idx, ':'))
				{
					token_idx++;
					if (strDataPointRequestType == String(F("GET")) || FreeAtHomeESPapi::GetStringToken(strDataPoint, strDataPointValue, token_idx, ':'))
					{
						String URI = FreeAtHomeESPapi::ConstructDeviceDataPointNotificationURI(strDataPointPart);
						//DEBUG_PL(URI); DEBUG_PL(strDataPointRequestType);	DEBUG_PL(strDataPointPart);

						if (!httpclt->HTTPRequestAsync(strDataPointRequestType, URI, strDataPointValue))
						{
							if (PendingDataPointsCount != MAX_PENDING_DATAPOINTS)
							{
								//Put it back, failed to send
								//But only if the que is not filled completly
								EnqueSetDataPoint(strDataPoint);
							}
							lastSendGap = 200;
							//DEBUG_P("ERROR_DPN:"); DEBUG_P(strDataPointPart); DEBUG_P(", Value:"); DEBUG_PL(strDataPointValue);
						}
					}
				}
			}
		}
	}
	if (lastSendGap > 0)
	{
		lastSendGap--;
	}
}

void FahESPDevice::process()
{	
	processBase();
}
