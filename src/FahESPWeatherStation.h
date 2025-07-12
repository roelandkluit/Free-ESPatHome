/*************************************************************************************************************
*
* Title			    : Free-ESPatHome
* Description:      : Library that implements the Busch-Jeager / ABB Free@Home API for ESP8266 and ESP32.
* Version		    : v 0.12
* Last updated      : 2024.07.19
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

class FahESPWeatherStation: public FahESPDevice
{
public:
	bool manualWindAlarmCreation = false;
	FahESPWeatherStation(const uint64_t& FahAbbID, const String& SerialNr, const uint16_t& timeout, FreeAtHomeESPapi* fahParent, FahSysAPInfo* SysApInfo);
	void process();
	~FahESPWeatherStation();
	static const String ConstStringDeviceType;
	void NotifyFahDataPoint(const String& strChannel, const String& strDataPoint, const String& strValue, const bool& isSceneOrGetValue);
	void NotifyDeviceParameter(const String& strChannel, const uint16_t& Parameter, const String& strValue);
	void SetBrightnessLevelWM2(const uint16_t &level);
	void SetBrightnessLevelByAnalogSensor(const uint8_t &Pin);
	void SetBrightnessLevelLux(const uint16_t& level, const bool& forceupdate = false);
	void SetRainInformation(const float& amount_of_rain, const bool& isRaining, const bool& forceupdate = false);
	void SetTemperatureLevel(const float& MessuredTemp, const bool& forceupdate = false);
	void SetWindSpeed(const uint8_t& speedBeaufort, const float& SpeedGustsMS, const bool& forceupdate = false);
	void SetWindSpeedBeaufort(const uint8_t& SpeedBeaufort, const bool& forceupdate = false);
	void SetWindAlarm(const bool& AlarmActive);
	void SetWindGustSpeed(const float& SpeedGustsMS, const bool& forceupdate = false);
private:
	float lvRain = -1;
	bool lvIsRaining = false;
	float lvTemperature = -60;
	uint16_t lvBrightness = 0xFFFF;
	float WindSpeedMS = -1;
	uint8_t uWindSpeedBeaufort = 0xFF;
	int alarmTemperature = 3;
	int alarmWindForce = 5;
};