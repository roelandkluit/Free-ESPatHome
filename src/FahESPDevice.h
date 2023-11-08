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
#pragma once
#include "BuildConfig.h"
#include "FahEventEnabledClass.h"
#include "FreeAtHomeESPapi.h"
#include "FahSysAPInfo.h"
#include "FahHTTPClient.h"

class FreeAtHomeESPapi;
class FahSysAPInfo;

#define MAX_PENDING_DATAPOINTS 10

class FahESPDevice : public FahEventEnabledClass
{
	protected:
		//FahSysAPInfo* SysAp = NULL;
		FreeAtHomeESPapi* SysApApi = NULL;
		FahHTTPClient* httpclt = NULL;
		String FahDeviceType;
		String SerialNr;
		uint64_t FahDevice = 0;
		bool EnqueDataPoint(const bool &GetValue, const String Channel, const String DataPoint, const String Value);
		bool EnqueSetDataPoint(const String Channel, const String DataPoint, const String Value);
		bool EnqueGetDataPoint(const String Channel, const String DataPoint);
		bool EnqueSetDataPoint(const String Entry);
		String GetJsonDeviceValueFromResponse(const String& Response);
		bool GetDataPointAndChannelFromURL(const String& URL, String& channel, String& datapoint);
	private:
		String* PendingDataPoints = NULL;
		uint8_t PendingDataPointsCount = 0;
		uint16_t TimeOut = 300;
		unsigned long WaitTimeMs = 0;
		FreeAtHomeESPapi* refParent = NULL;
		unsigned long LastWaitInterval = 0;
		String DequeDataPoint();
		uint8_t lastSendGap = 0;		
	public:
		uint8_t GetPendingDatapointCount() { return PendingDataPointsCount; };
		unsigned long GetMScounter() { return LastWaitInterval; };
		virtual void NotifyFahDataPoint(const String& strChannel, const String& strDataPoint, const String& strValue, const bool& isSceneOrGetValue);
		uint64_t GetFahDeviceID();
		FahESPDevice(const String& FahDeviceType, const uint64_t& FahAbbID, const String& SerialNr, const uint16_t& timeout, FreeAtHomeESPapi* fahParent, FahSysAPInfo* SysApInfo);
		~FahESPDevice();		
		virtual void process();
		void processBase();
};

