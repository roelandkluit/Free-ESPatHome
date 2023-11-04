/*************************************************************************************************************
*
* Title			    : Free-ESPatHome
* Description:      : Library that implements the Busch-Jeager / ABB Free@Home API for ESP8266 and ESP32.
* Version		    : v 0.5
* Last updated      : 2023.11.04
* Target		    : ESP32, ESP8266, ESP8285
* Author            : Roeland Kluit
* Web               : https://github.com/roelandkluit/Free-ESPatHome
* License           : GPL-3.0 license
*
**************************************************************************************************************/
#include "FahESPDevice.h"

bool FahESPDevice::EnqueDataPoint(const String Channel, const String DataPoint, const String Value)
{
	String sDeviceID = FreeAtHomeESPapi::U64toString(FahDevice);
	String Entry = sDeviceID + "." + Channel + "." + DataPoint + ":" + Value;
	return EnqueDataPoint(Entry);
}

bool FahESPDevice::EnqueDataPoint(const String Entry)
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

void FahESPDevice::NotifyFahDataPoint(const String& strChannel, const String& strDataPoint, const String& strValue, const bool& isScene)
{
}

uint64_t FahESPDevice::GetFahDeviceID()
{
	return FahDevice;
}

FahESPDevice::FahESPDevice(const String& FahDeviceType, const uint64_t& FahAbbID, const String& SerialNr, const uint16_t& timeout, FreeAtHomeESPapi* fahParent, FahSysAPInfo* SysApInfo)
{
	httpclt = new FahHTTPClient(SysApInfo);
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
		httpclt->ReleaseAsync();
		lastSendGap = 200;
	}
	else if(httpclt->GetAsyncStatus() == HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_SUCCESS)
	{		
		//String returndata = httpclt->GetBody();
		//DEBUG_PL(returndata);
		//DEBUG_PL(F("HTTP_ASYNC: OK"));
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
			String strDataPointPart;
			String strDataPointValue;

			uint8_t token_idx = 0;
			if (FreeAtHomeESPapi::GetStringToken(strDataPoint, strDataPointPart, token_idx, ':'))
			{
				token_idx++;
				if (FreeAtHomeESPapi::GetStringToken(strDataPoint, strDataPointValue, token_idx, ':'))
				{
					String URI = FreeAtHomeESPapi::ConstructDeviceDataPointNotificationURI(strDataPointPart);
					if (!httpclt->HTTPRequestAsync(String(F("PUT")), URI, strDataPointValue))
					{
						if (PendingDataPointsCount != MAX_PENDING_DATAPOINTS)
						{
							//Put it back, failed to send
							//But only if the que is not filled completly
							EnqueDataPoint(strDataPoint);
						}
						lastSendGap = 200;

						/*DEBUG_P("ERROR_DPN:");
						DEBUG_P(strDataPointPart);
						DEBUG_P(", Value:");
						DEBUG_PL(strDataPointValue);
						DEBUG_PL(URI);
						*/
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
