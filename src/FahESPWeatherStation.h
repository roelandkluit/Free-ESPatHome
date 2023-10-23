/*************************************************************************************************************
*
* Title			    : Free-ESPatHome
* Description:      : Library that implements the Busch-Jeager / ABB Free@Home API for ESP8266 and ESP32.
* Version		    : v 0.3
* Last updated      : 2023.10.23
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

class FahESPWeatherStation: public FahESPDevice
{
public:
	FahESPWeatherStation(const uint64_t& FahAbbID, const String& SerialNr, const uint16_t& timeout, FreeAtHomeESPapi* fahParent, FahSysAPInfo* SysApInfo);
	void process();
	~FahESPWeatherStation();
	static const String ConstStringDeviceType;
	void NotifyFahDataPoint(const String& strChannel, const String& strDataPoint, const String& strValue, const bool& isScene);
	void SetBrightnessLevelWM2(uint16_t level);
	void SetBrightnessLevelByAnalogSensor(uint8_t Pin);
	void SetBrightnessLevelLux(uint16_t level);
	void SetRainInformation(double amount_of_rain);
	void SetTemperatureLevel(double level);
	void SetWindSpeed(uint8_t speedBaufort, uint8_t speedMS);
private:
	double lvRain = -1;
	double lvTemperature = 0;
	uint16_t lvBrightness = 0;
	uint8_t lvWindSpeed = 0;
	uint8_t lvWindBeaufort = 0;
};