/*************************************************************************************************************
*
* Title			    : Free-ESPatHome
* Description:      : Library that implements the Busch-Jeager / ABB Free@Home API for ESP8266 and ESP32.
* Version		    : v 0.7
* Last updated      : 2023.12.05
* Target		    : ESP32, ESP8266, ESP8285
* Author            : Roeland Kluit
* Web               : https://github.com/roelandkluit/Free-ESPatHome
* License           : GPL-3.0 license
*
**************************************************************************************************************/
#pragma once
#include "FahESPBuildConfig.h"
#include "FahESPDevice.h"

class FreeAtHomeESPapi;
class FahSysAPInfo;

class FahESPSwitchDevice: public FahESPDevice
{
public:
	FahESPSwitchDevice(const uint64_t& FahAbbID, const String& SerialNr, const uint16_t& timeout, FreeAtHomeESPapi* fahParent, FahSysAPInfo* SysApInfo);
	void process();
	bool GetState();
	~FahESPSwitchDevice();
	static const String ConstStringDeviceType;
	void NotifyFahDataPoint(const String& strChannel, const String& strDataPoint, const String& strValue, const bool& isSceneOrGetValue);
	void SetState(bool isOn);
	//void SetOnDeviceOnOffEvent(void(*callback)(FahESPSwitchDevice* Caller, const bool& isOn)) { CALLBACK_DEVICE_ONOFF_EVENT = callback; }
private:
	//void(*CALLBACK_DEVICE_ONOFF_EVENT)(FahESPSwitchDevice* Caller, const bool& isOn) = NULL;
	bool isOn = false;

};