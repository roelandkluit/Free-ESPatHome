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
#include "FahEventEnabledClass.h"

void FahEventEnabledClass::AddCallback(FREEATHOME_EVENT_CALLBACK __CALLBACK)
{
	if (callbackcount == 10)
		return;
	callbackcount++;
	FREEATHOME_EVENT_CALLBACK* newCallbacks = new FREEATHOME_EVENT_CALLBACK[callbackcount];
	for (uint8_t i = 0; i < callbackcount - 1; i++)
	{
		//bool succesResult;
		newCallbacks[i] = callbacks[i];
	}
	newCallbacks[callbackcount - 1] = __CALLBACK;
	if (callbacks != NULL)
		delete[] callbacks;

	callbacks = newCallbacks;
}

void FahEventEnabledClass::NotifyCallback(FAHESPAPI_EVENT Event, uint64_t FAHID, const char* ptrChannel, const char* ptrDataPoint, void* ptrValue)
{
	for (uint8_t i = 0; i < callbackcount; i++)
	{
		callbacks[i](Event, FAHID, ptrChannel, ptrDataPoint, ptrValue);
	}
}

FahEventEnabledClass::~FahEventEnabledClass()
{
	if (callbackcount > 0)
	{
		delete[] callbacks;
		callbacks = NULL;
	}
}

FahEventEnabledClass::FahEventEnabledClass()
{
	callbackcount = 0;
	callbacks = NULL;
}
