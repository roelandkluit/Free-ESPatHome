/*************************************************************************************************************
*
* Title			    : Free-ESPatHome
* Description:      : Library that implements the Busch-Jeager / ABB Free@Home API for ESP8266 and ESP32.
* Version		    : v 0.10
* Last updated      : 2023.12.13
* Target		    : ESP32, ESP8266, ESP8285
* Author            : Roeland Kluit
* Web               : https://github.com/roelandkluit/Free-ESPatHome
* License           : GPL-3.0 license
*
**************************************************************************************************************/
#include "FahESPWeatherStation.h"
#include "FahParamDefinitions.h"

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

void FahESPWeatherStation::NotifyFahDataPoint(const String& strChannel, const String& strDataPoint, const String& strValue, const bool& isSceneOrGetValue)
{
	//No events needed
}

void FahESPWeatherStation::NotifyDeviceParameter(const String& strChannel, const uint16_t& Parameter, const String& strValue)
{
	//DEBUG_P(String(F("NDP:"))); DEBUG_P(strChannel); DEBUG_P('>'); DEBUG_P(Parameter); DEBUG_P('>'); DEBUG_PL(strValue);
	if(FreeAtHomeESPapi::GetChannelString(2) == strChannel)
	{
		if(Parameter == PID_FROST_ALARM_ACTIVATION_LEVEL)
		{
			int lAlarmValue = strValue.toInt();
			if (lAlarmValue != this->alarmTemperature)
			{
				DEBUG_PL(String(F("newTempAlarm")));
				this->alarmTemperature = lAlarmValue;
			}
		}
	}
	if (FreeAtHomeESPapi::GetChannelString(3) == strChannel)
	{
		if (Parameter == PID_WIND_FORCE)
		{
			int lAlarmValue = strValue.toInt();
			if (lAlarmValue != this->alarmWindForce)
			{
				DEBUG_PL(String(F("newWindAlarm")));
				this->alarmWindForce = lAlarmValue;
			}
		}
	}
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

void FahESPWeatherStation::SetBrightnessLevelLux(const uint16_t &level, const bool &forceupdate)
{
	if (!forceupdate && level == lvBrightness)
		return;
	else
	{
		//Level
		String Body1 = String(level);
		if (EnqueSetDataPoint(FreeAtHomeESPapi::GetChannelString(0), FreeAtHomeESPapi::GetODPString(1), Body1))
		{
			lvBrightness = level;
		}
	}
}

void FahESPWeatherStation::SetRainInformation(const float &amount_of_rain, const bool &isRaining, const bool& forceupdate)
{
	if (!forceupdate && lvRain == amount_of_rain)
		return;
	else
	{
		//Alarm
		String Body1 = FreeAtHomeESPapi::VALUE_0;
		if (amount_of_rain > 0 || isRaining)
		{
			Body1 = FreeAtHomeESPapi::VALUE_1;
		}
		EnqueSetDataPoint(FreeAtHomeESPapi::GetChannelString(1), FreeAtHomeESPapi::GetODPString(0), Body1);

		//Rain
		String Body2 = String(amount_of_rain);
		if (EnqueSetDataPoint(FreeAtHomeESPapi::GetChannelString(1), FreeAtHomeESPapi::GetODPString(2), Body2))
		{
			lvRain = amount_of_rain;
		}
	}
}

void FahESPWeatherStation::SetTemperatureLevel(const float &MessuredTemp, const bool& forceupdate)
{
	if (!forceupdate && lvTemperature == MessuredTemp)
		return;
	else
	{
		//Alarm
		String Body1 = FreeAtHomeESPapi::VALUE_0;
		if (MessuredTemp <= alarmTemperature)
		{
			Body1 = FreeAtHomeESPapi::VALUE_1;
		}
		EnqueSetDataPoint(FreeAtHomeESPapi::GetChannelString(2), FreeAtHomeESPapi::GetODPString(0), Body1);

		//Value
		String Body2 = String(MessuredTemp);
		if (EnqueSetDataPoint(FreeAtHomeESPapi::GetChannelString(2), FreeAtHomeESPapi::GetODPString(1), Body2))
		{
			lvTemperature = MessuredTemp;
		}
	}
}

void FahESPWeatherStation::SetWindSpeed(const uint8_t& speedBeaufort, const float& SpeedGustsMS, const bool& forceupdate)
{
	SetWindSpeedBeaufort(speedBeaufort, forceupdate);
	SetWindGustSpeed(SpeedGustsMS, forceupdate);
}

void FahESPWeatherStation::SetWindSpeedBeaufort(const uint8_t& SpeedBeaufort, const bool& forceupdate)
{
	if (!forceupdate && uWindSpeedBeaufort != SpeedBeaufort)
	{
		//Alarm
		String Body1 = FreeAtHomeESPapi::VALUE_0;
		if (SpeedBeaufort >= alarmWindForce)
		{
			Body1 = FreeAtHomeESPapi::VALUE_1;
		}
		EnqueSetDataPoint(FreeAtHomeESPapi::GetChannelString(3), FreeAtHomeESPapi::GetODPString(0), Body1);

		//Speed
		String Body2 = String(SpeedBeaufort);
		if (EnqueSetDataPoint(FreeAtHomeESPapi::GetChannelString(3), FreeAtHomeESPapi::GetODPString(1), Body2))
		{
			uWindSpeedBeaufort = SpeedBeaufort;
		}
	}
}

void FahESPWeatherStation::SetWindGustSpeed(const float& SpeedGustsMS, const bool& forceupdate)
{
	if (!forceupdate && WindSpeedMS != SpeedGustsMS)
	{
		//Speed M/S
		String Body3 = String(SpeedGustsMS, 2);
		if (EnqueSetDataPoint(FreeAtHomeESPapi::GetChannelString(3), FreeAtHomeESPapi::GetODPString(3), Body3))
		{
			WindSpeedMS = SpeedGustsMS;
		}
	}
}
