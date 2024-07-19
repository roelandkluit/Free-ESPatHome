/*************************************************************************************************************
*
* Title			    : Free-ESPatHome
* Description:      : Library that implements the Busch-Jeager / ABB Free@Home API for ESP8266 and ESP32.
* Version		    : v 0.12
* Last updated      : 2023.12.13
* Target		    : ESP32, ESP8266, ESP8285
* Author            : Roeland Kluit
* Web               : https://github.com/roelandkluit/Free-ESPatHome
* License           : GPL-3.0 license
*
**************************************************************************************************************/
#pragma once
#include <Arduino.h>
#include "FahESPBuildConfig.h"

namespace FAHESPAPI_EVENTS
{
	enum FAHESPAPI_EVENT :uint8_t
	{
		FAHESPAPI_NONE = 0,
		FAHESPAPI_NEED_INFO = 1,
		FAHESPAPI_ON_DATAPOINT = 2,
		FAHESPAPI_ON_DEVICE_EVENT = 3,
		FAHESPAPI_ON_DISPLAYNAME = 4,
	};
}

typedef FAHESPAPI_EVENTS::FAHESPAPI_EVENT FAHESPAPI_EVENT;
typedef void (*FREEATHOME_EVENT_CALLBACK)(FAHESPAPI_EVENT Event, uint64_t FAHID, const char* ptrChannel, const char* ptrDataPoint, void* ptrValue);

class FahEventEnabledClass
{
public:
	~FahEventEnabledClass();
	FahEventEnabledClass();
	void AddCallback(FREEATHOME_EVENT_CALLBACK __CALLBACK);	
protected:
	void NotifyCallback(FAHESPAPI_EVENT Event, uint64_t FAHID, const char* ptrChannel, const char* ptrDataPoint, void* ptrValue);	
	FREEATHOME_EVENT_CALLBACK* callbacks;
	uint8_t callbackcount;
private:

};