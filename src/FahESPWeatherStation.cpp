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
#include "FahESPWeatherStation.h"

const String FahESPWeatherStation::ConstStringDeviceType = "WeatherStation";

FahESPWeatherStation::FahESPWeatherStation(const uint64_t& FahAbbID, const String& SerialNr, const uint16_t& timeout, FreeAtHomeESPapi* fahParent, FahSysAPInfo* SysApInfo):FahESPDevice(ConstStringDeviceType, FahAbbID, SerialNr, timeout, fahParent, SysApInfo)
{
}

void FahESPWeatherStation::process()
{
	processBase();
}

FahESPWeatherStation::~FahESPWeatherStation()
{
}

void FahESPWeatherStation::NotifyFahDataPoint(const String& strChannel, const String& strDataPoint, const String& strValue, const bool& isScene)
{
	//No events needed
}

void FahESPWeatherStation::SetBrightnessLevelWM2(const uint16_t &level)
{
	uint16_t ilevel = level * 63;
	SetBrightnessLevelLux(ilevel);
}

void FahESPWeatherStation::SetBrightnessLevelByAnalogSensor(const uint8_t &Pin)
{
	uint16_t val = analogRead(Pin);
	val = val * ((val / 10) + 1);
	SetBrightnessLevelLux(val);
}

void FahESPWeatherStation::SetBrightnessLevelLux(const uint16_t &level)
{
	if (level == lvBrightness)
		return;
	else
	{
		//Level
		String Body1 = String(level);
		if (EnqueDataPoint(FreeAtHomeESPapi::GetChannelString(0), FreeAtHomeESPapi::GetODPString(1), Body1))
		{
			lvBrightness = level;
		}
	}
}

void FahESPWeatherStation::SetRainInformation(const double &amount_of_rain)
{
	if (lvRain == amount_of_rain)
		return;
	else
	{
		//Alarm
		String Body1 = FreeAtHomeESPapi::VALUE_0;
		if (amount_of_rain > 0)
		{
			Body1 = FreeAtHomeESPapi::VALUE_1;
		}
		EnqueDataPoint(FreeAtHomeESPapi::GetChannelString(1), FreeAtHomeESPapi::GetODPString(0), Body1);

		//Rain
		String Body2 = String(amount_of_rain);
		if (EnqueDataPoint(FreeAtHomeESPapi::GetChannelString(1), FreeAtHomeESPapi::GetODPString(2), Body2))
		{
			lvRain = amount_of_rain;
		}
	}
}

void FahESPWeatherStation::SetTemperatureLevel(const double &MessuredTemp)
{
	if (lvTemperature == MessuredTemp)
		return;
	else
	{
		//Alarm
		String Body1 = FreeAtHomeESPapi::VALUE_0;
		if (MessuredTemp <= 0)
		{
			Body1 = FreeAtHomeESPapi::VALUE_1;
		}
		EnqueDataPoint(FreeAtHomeESPapi::GetChannelString(2), FreeAtHomeESPapi::GetODPString(0), Body1);

		//Value
		String Body2 = String(MessuredTemp);
		if (EnqueDataPoint(FreeAtHomeESPapi::GetChannelString(2), FreeAtHomeESPapi::GetODPString(1), Body2))
		{
			lvTemperature = MessuredTemp;
		}
	}
}


void FahESPWeatherStation::SetWindSpeed(const uint8_t& speedBeaufort, const float& SpeedGustsMS)
{
	SetWindSpeedBeaufort(speedBeaufort);
	SetWindGustSpeed(SpeedGustsMS);
}

void FahESPWeatherStation::SetWindSpeedBeaufort(const uint8_t& SpeedBeaufort)
{
	if (uWindSpeedBeaufort != SpeedBeaufort)
	{
		//Alarm
		String Body1 = FreeAtHomeESPapi::VALUE_0;
		if (SpeedBeaufort > 3)
		{
			Body1 = FreeAtHomeESPapi::VALUE_1;
		}
		EnqueDataPoint(FreeAtHomeESPapi::GetChannelString(3), FreeAtHomeESPapi::GetODPString(0), Body1);

		//Speed
		String Body2 = String(SpeedBeaufort);
		if (EnqueDataPoint(FreeAtHomeESPapi::GetChannelString(3), FreeAtHomeESPapi::GetODPString(1), Body2))
		{
			uWindSpeedBeaufort = SpeedBeaufort;
		}
	}
}

void FahESPWeatherStation::SetWindGustSpeed(const float& SpeedGustsMS)
{
	if (WindSpeedMS != SpeedGustsMS)
	{
		//Speed M/S
		String Body3 = String(SpeedGustsMS, 2);
		if (EnqueDataPoint(FreeAtHomeESPapi::GetChannelString(3), FreeAtHomeESPapi::GetODPString(3), Body3))
		{
			WindSpeedMS = SpeedGustsMS;
		}
	}
}
