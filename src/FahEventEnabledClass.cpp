/*
Free-ESPAtHome
Copyright 2023 Roeland Kluit, GPL License
Implements the Busch-Jeager / ABB Free@Home API for ESP8266 and ESP32.
*/
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
