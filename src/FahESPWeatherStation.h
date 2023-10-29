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
	void SetRainInformation(const double &amount_of_rain);
	void SetTemperatureLevel(const double &level);
	void SetWindSpeed(const uint8_t &speedBaufort, const float &speedMS);
	void SetWindSpeedBeaufort(const uint8_t &speedBaufort);
	void SetWindGustSpeed(const float &speedMS);
private:
	double lvRain = -1;
	double lvTemperature = 0;
	uint16_t lvBrightness = 0;
	uint8_t WindSpeedMS = 0;
	uint8_t uWindSpeedBeaufort = 0;
};