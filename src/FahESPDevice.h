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
#include "FahEventEnabledClass.h"
#include "FreeAtHomeESPapi.h"
#include "FahSysAPInfo.h"
#include "FahHTTPClient.h"

class FreeAtHomeESPapi;
class FahSysAPInfo;

class FahESPDevice : public FahEventEnabledClass
{
	protected:
		FahSysAPInfo* SysAp = NULL;
		FahHTTPClient* httpclt = NULL;
		String FahDeviceType;
		String SerialNr;
		uint64_t FahDevice = 0;
		bool EnqueDataPoint(const String Channel, const String DataPoint, const String Value);		
	private:
		String* PendingDataPoints = NULL;
		uint8_t PendingDataPointsCount = 0;
		uint16_t TimeOut = 300;
		uint16_t WaitTime = 0;
		FreeAtHomeESPapi* refParent = NULL;
		uint16_t LastSecondsInterval = 0;
		String DequeDataPoint();
	public:
		virtual void NotifyFahDataPoint(const String& strChannel, const String& strDataPoint, const String& strValue, const bool& isScene);
		uint64_t GetFahDeviceID();
		FahESPDevice(const String& FahDeviceType, const uint64_t& FahAbbID, const String& SerialNr, const uint16_t& timeout, FreeAtHomeESPapi* fahParent, FahSysAPInfo* SysApInfo);
		~FahESPDevice();
		virtual void process();
		void processBase();
};

