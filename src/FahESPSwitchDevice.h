/*************************************************************************************************************
*
* Title			    : Free-ESPatHome
* Description:      : Library that implements the Busch-Jeager / ABB Free@Home API for ESP8266 and ESP32.
* Version		    : v 0.2
* Last updated      : 2023.10.20
* Target		    : ESP32, ESP8266, ESP8285
* Author            : Roeland Kluit
* Web               : https://github.com/roelandkluit/Free-ESPatHome
* License           : GPL-3.0 license
*
**************************************************************************************************************/
#pragma once
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
	void NotifyFahDataPoint(const String& strChannel, const String& strDataPoint, const String& strValue, const bool& isScene);
	void SetState(bool isOn);
private:
	bool isOn = false;

};