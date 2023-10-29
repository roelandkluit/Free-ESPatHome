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
	void SetBrightnessLevelWM2(const uint16_t &level);
	void SetBrightnessLevelByAnalogSensor(const uint8_t &Pin);
	void SetBrightnessLevelLux(const uint16_t &level);
	void SetRainInformation(const float &amount_of_rain);
	void SetTemperatureLevel(const float &level);
	void SetWindSpeed(const uint8_t &speedBaufort, const float &speedMS);
	void SetWindSpeedBeaufort(const uint8_t &speedBaufort);
	void SetWindGustSpeed(const float &speedMS);
private:
	float lvRain = -1;
	float lvTemperature = -60;
	uint16_t lvBrightness = 0xFFFF;
	float WindSpeedMS = -1;
	uint8_t uWindSpeedBeaufort = 0xFF;
};