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
#include "FahESPBuildConfig.h"
#include "FahESPSwitchDevice.h"

const String FahESPSwitchDevice::ConstStringDeviceType = "SwitchingActuator";

FahESPSwitchDevice::FahESPSwitchDevice(const uint64_t& FahAbbID, const String& SerialNr, const uint16_t& timeout, FreeAtHomeESPapi* fahParent, FahSysAPInfo* SysApInfo):FahESPDevice(ConstStringDeviceType, FahAbbID, SerialNr, timeout, fahParent, SysApInfo)
{	
	//Functionality identical when reconnect of SysAP
	NotifyOnSysAPReconnect();
}

void FahESPSwitchDevice::process()
{
	processBase();
}

bool FahESPSwitchDevice::GetState()
{
	return this->isOn;
}


void FahESPSwitchDevice::SetState(bool isOn)
{
	this->isOn = isOn;
	String Body = FreeAtHomeESPapi::VALUE_0;
	if (isOn)
	{
		Body = FreeAtHomeESPapi::VALUE_1;
	}
	EnqueSetDataPoint(FreeAtHomeESPapi::GetChannelString(0), FreeAtHomeESPapi::GetODPString(0), Body);
	this->NotifyCallback(FAHESPAPI_EVENT::FAHESPAPI_ON_DEVICE_EVENT, this->FahDevice, FreeAtHomeESPapi::GetChannelString(0).c_str(), String(F("ON")).c_str(), (void*)isOn);
}

FahESPSwitchDevice::~FahESPSwitchDevice()
{
}

void FahESPSwitchDevice::NotifyOnSysAPReconnect()
{
	//Ensure to retrieve device state again after reconnect of SysAp
	//Same procedure as after device registration
	EnqueGetDataPoint(FreeAtHomeESPapi::GetChannelString(0), FreeAtHomeESPapi::GetODPString(0));
}

void FahESPSwitchDevice::NotifyFahDataPoint(const String& strChannel, const String& strDataPoint, const String& strValue, const bool& isSceneOrGetValue)
{
	//DEBUG_P("VDN:");	DEBUG_P(strChannel); DEBUG_P("."); DEBUG_P(strDataPoint); DEBUG_P("=");	DEBUG_PL(strValue);
	if (strChannel == FreeAtHomeESPapi::GetChannelString(0))
	{
		if (strDataPoint == FreeAtHomeESPapi::GetIDPString(0) || (isSceneOrGetValue && strDataPoint == FreeAtHomeESPapi::GetODPString(0)))
		{
			if (strValue == FreeAtHomeESPapi::VALUE_0)
			{
				SetState(false);
			}
			else
			{
				SetState(true);
			}
		}
	}
}