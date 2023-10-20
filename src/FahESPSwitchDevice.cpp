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
#include "FahESPSwitchDevice.h"

const String FahESPSwitchDevice::ConstStringDeviceType = "SwitchingActuator";

FahESPSwitchDevice::FahESPSwitchDevice(const uint64_t& FahAbbID, const String& SerialNr, const uint16_t& timeout, FreeAtHomeESPapi* fahParent, FahSysAPInfo* SysApInfo):FahESPDevice(ConstStringDeviceType, FahAbbID, SerialNr, timeout, fahParent, SysApInfo)
{	
	//Set initial state, todo: change to parameter state request 
	/*
	    string dataPointValue = client.DownloadString("https://" + EndPointURL + "/fhapi/v1/api/rest/datapoint/00000000-0000-0000-0000-000000000000/" + dataPoint.device + "." + dataPoint.channel + "." + dataPoint.datapoint);
        JArray jsonArraydataPointValue = (JArray)JObject.Parse(dataPointValue)["00000000-0000-0000-0000-000000000000"]["values"];
	*/
	SetState(0);
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
	String Body = "0";
	if (isOn)
	{
		Body = "1";
	}
	EnqueDataPoint(FreeAtHomeESPapi::KEY_CHANEL0, FreeAtHomeESPapi::KEY_ODP0000, Body);
	this->NotifyCallback(FAHESPAPI_EVENT::FAHESPAPI_ON_DEVICE_EVENT, this->FahDevice, FreeAtHomeESPapi::KEY_CHANEL0.c_str(), String(F("ON")).c_str(), (void*)isOn);
}

FahESPSwitchDevice::~FahESPSwitchDevice()
{
}

void FahESPSwitchDevice::NotifyFahDataPoint(const String& strChannel, const String& strDataPoint, const String& strValue, const bool& isScene)
{
	//DEBUG_P("VDN:");	DEBUG_P(strChannel); DEBUG_P("."); DEBUG_P(strDataPoint); DEBUG_P("=");	DEBUG_PL(strValue);
	if (strChannel == FreeAtHomeESPapi::KEY_CHANEL0)
	{
		if (strDataPoint == FreeAtHomeESPapi::KEY_IDP0000 || (isScene && strDataPoint == FreeAtHomeESPapi::KEY_ODP0000))
		{
			if (strValue == "0")
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