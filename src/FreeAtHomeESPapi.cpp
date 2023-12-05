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
#include "FreeAtHomeESPapi.h"
#include "FahESPSwitchDevice.h"
#include <base64.h>
#include "FahESPWeatherStation.h"

const String FreeAtHomeESPapi::KEY_ROOT = "00000000-0000-0000-0000-000000000000";
const String FreeAtHomeESPapi::KEY_DATAPOINTS = "datapoints";
const String FreeAtHomeESPapi::KEY_CHANNELS = "channels";
const String FreeAtHomeESPapi::KEY_OUTPUTS = "outputs";
const String FreeAtHomeESPapi::KEY_INPUTS = "inputs";
const String FreeAtHomeESPapi::KEY_VALUE = "value";
const String FreeAtHomeESPapi::KEY_VALUES = "values";
const String FreeAtHomeESPapi::KEY_DEVICES = "devices";
const String FreeAtHomeESPapi::KEY_DISPLAYNAME = "displayName";
const String FreeAtHomeESPapi::KEY_SCENESTRIGGERED = "scenesTriggered";
const String FreeAtHomeESPapi::KEY_PARAMETERS = "parameters";
const String FreeAtHomeESPapi::VALUE_0 = "0";
const String FreeAtHomeESPapi::VALUE_1 = "1";

String FreeAtHomeESPapi::GetIDPString(const uint8_t &Number)
{
	String hexNr = String(Number, HEX);
	hexNr.toUpperCase();
	return String(F("idp0000")).substring(0, 7 - hexNr.length()) + hexNr;
}

String FreeAtHomeESPapi::GetChannelString(const uint8_t &Number)
{
	String hexNr = String(Number, HEX);
	hexNr.toUpperCase();
	return String(F("ch0000")).substring(0, 6 - hexNr.length()) + hexNr;
}

String FreeAtHomeESPapi::GetODPString(const uint8_t &Number)
{
	String hexNr = String(Number, HEX);
	hexNr.toUpperCase();
	return String(F("odp0000")).substring(0, 7 - hexNr.length()) + hexNr;
}

uint64_t FreeAtHomeESPapi::StringDevToU64(const String& DeviceID)
{
	String substr;
	substr = DeviceID.substring(0, 6);
	uint64_t p1 = ((uint64_t)strtoul(substr.c_str(), 0, 16)) << 24;
	substr = DeviceID.substring(6, 12);
	uint64_t p2 = strtoul(substr.c_str(), 0, 16);

	uint64_t t = p1 + p2;
	return t;
}

String FreeAtHomeESPapi::U64toString(const uint64_t number)
{
	String stringout;
	U64toStringDev(number, stringout);
	return stringout;
}

String FreeAtHomeESPapi::GetPadString(const String& refString, const uint8_t& size, const char& padChar)
{
	if (refString.length() < size)
	{
		uint8_t padCharsCount = size - refString.length();
		String pad = "";
		pad.reserve(padCharsCount);
		while (padCharsCount > 0)
		{
			pad += padChar;
			padCharsCount--;
		}
		return pad;
	}
	return "";
}

void FreeAtHomeESPapi::U64toStringDev(const uint64_t number, String& stringref)
{
	long p1 = number >> 24;
	long p2 = number & 0xFFFFFF;

	String ps1 = String(p1, HEX);
	String ps2 = String(p2, HEX);

	stringref = GetPadString(ps1, 6, '0') + ps1 + GetPadString(ps2, 6, '0') + ps2;
	stringref.toUpperCase();
}

FreeAtHomeESPapi::FreeAtHomeESPapi()
{
	callbacks = NULL;	
	ws = NULL;
}

FreeAtHomeESPapi::~FreeAtHomeESPapi()
{
	if (SysApInfo != NULL)
	{
		delete SysApInfo;
		SysApInfo = NULL;
	}
	if (ws != NULL)
	{
		delete ws;
		ws = NULL;
	}
}

bool FreeAtHomeESPapi::RegisterFahEspDevice(FahESPDevice *Device)
{
	for (uint8_t i = 0; i < MAX_ESP_CREATED_DEVICES; i++)
	{
		if(EspDevices[i] == NULL)
		{
			if (Device != NULL)
			{
				EspDevices[i] = Device;
			}
			return true;
		}
	}
	return false;
}

bool FreeAtHomeESPapi::RemoveDevice(FahESPDevice* Device)
{
	for (uint8_t i = 0; i < MAX_ESP_CREATED_DEVICES; i++)
	{
		if (EspDevices[i] == Device)
		{
			delete Device;
			EspDevices[i] = NULL;
			return true;
		}
	}
	return false;
}

FahESPSwitchDevice* FreeAtHomeESPapi::CreateSwitchDevice(const String& SerialNr, const String& DisplayName, const uint16_t& timeout)
{
	return (FahESPSwitchDevice*)CreateDevice(SerialNr, FahESPSwitchDevice::ConstStringDeviceType, DisplayName, timeout);
}

FahESPWeatherStation* FreeAtHomeESPapi::CreateWeatherStation(const String& SerialNr, const String& DisplayName, const uint16_t& timeout)
{
	return (FahESPWeatherStation*)CreateDevice(SerialNr, FahESPWeatherStation::ConstStringDeviceType, DisplayName, timeout);
}

FahESPDevice* FreeAtHomeESPapi::CreateDevice(const String& SerialNr, const String& deviceType, const String& DisplayName, const uint16_t& timeout)
{
	if (!RegisterFahEspDevice(NULL))
	{
		//No slots available
		return NULL;
	}

	if (ws == NULL)
		return NULL;
	if (!ws->isConnected())
		return NULL;

	FahHTTPClient* httpclt = new FahHTTPClient(this->SysApInfo);
	String URI = FreeAtHomeESPapi::ConstructDeviceRegistrationURI(SerialNr);
	String HTTPPostData = FreeAtHomeESPapi::ConstructDeviceRegistrationBody(deviceType, DisplayName, timeout);

	if (!httpclt->HTTPRequest(String(F("PUT")), URI, HTTPPostData))
	{
		DEBUG_PL(F("Registration Failed"));		
		return NULL;
	}

	uint64_t OutFahID;
	String Body = httpclt->GetBody();
	if (this->ProcessJsonData(Body, JsonProcessFilter::PROCESS_DEVICES, &OutFahID))
	{
		FahESPDevice* outDevice = NULL;
		if (deviceType == FahESPSwitchDevice::ConstStringDeviceType)
		{
			outDevice = new FahESPSwitchDevice(OutFahID, SerialNr, timeout, this, this->SysApInfo);
		}
		else if (deviceType == FahESPWeatherStation::ConstStringDeviceType)
		{
			outDevice = new FahESPWeatherStation(OutFahID, SerialNr, timeout, this, this->SysApInfo);
		}
		else
		{
			outDevice = new FahESPDevice(deviceType, OutFahID, SerialNr, timeout, this, this->SysApInfo);
		}
		RegisterFahEspDevice(outDevice);
		return outDevice;
	}
	return NULL;
}

bool FreeAtHomeESPapi::ConnectToSysAP(const String& SysAPHostname, const String& Username, const String& Password, const bool& useSSL)
{
	String encoded = String(F("Basic ")) + base64::encode(Username + ":" + Password);
	return ConnectToSysAP(SysAPHostname, encoded, useSSL);
}

bool FreeAtHomeESPapi::ConnectToSysAP(const String& SysAPHostname, const String& AuthorizationHeader, const bool& useSSL)
{
	#ifdef ESP8266
		#ifndef FORCE_ESP8266_SSL_OPTION_AVAILBLE
			// the option useSSL does not work with an ESP8266 with multiple SSL sessions (WebSoscket and calls) due to memory restrictions
			// use a ESP32 when using SSL with FaH
		if (useSSL)
		{
			DEBUG_PL(String(F("useSSL does not work with an ESP8266")));
			return false;
		}
		#endif // ! FORCE_ESP8266_SSL_OPTION_AVAILBLE
	#endif // ESP8266
	
	if (ws != NULL)
	{
		if (ws->isConnected()) //Do not attempt to connect if a connection exists
		{
			return true;
		}
		else //Connection closed, delete socket
		{
			delete ws;
			ws = NULL;
			//return false;
		}
	}

	if (this->SysApInfo == NULL)
	{
		this->SysApInfo = new FahSysAPInfo();
	}
	this->SysApInfo->Hostname = SysAPHostname;
	this->SysApInfo->authorizationHeader = AuthorizationHeader;
	this->SysApInfo->secure = useSSL;
	if (this->SysApInfo->secure)
		this->SysApInfo->port = 443;
	else
		this->SysApInfo->port = 80;

	ws = new WebSocketClient(useSSL);
	ws->setAuthorizationHeader(AuthorizationHeader);
	
	if(!ws->connect(SysAPHostname, F("/fhapi/v1/api/ws"), this->SysApInfo->port))
	{
		delete ws;
		ws = NULL;
		return false;
	}
	else
	{
		return true;
	}
}

bool FreeAtHomeESPapi::ProcessJsonData(String& recievedData, JsonProcessFilter filter, uint64_t* hexDeviceOut)
{
	bool retval = false;
	if (recievedData.length() == 0)
		return retval;

	DynamicJsonDocument fahJsonMsg(MAX_ARDUINOJSON_DOC_SIZE);
	DeserializationError error = deserializeJson(fahJsonMsg, recievedData);

	if (error)
	{
		// Test if parsing succeeds.
		DEBUG_P(F("deserializeJson() failed: ")); DEBUG_PL(error.f_str());
		DEBUG_PL(recievedData);
	}
	else
	{
		if (fahJsonMsg.containsKey(KEY_ROOT))
		{
			JsonObject root = fahJsonMsg[KEY_ROOT].as<JsonObject>();
			JsonObject outDP;

			if ((filter & JsonProcessFilter::PROCESS_DATAPOINTS) == JsonProcessFilter::PROCESS_DATAPOINTS)
			{
				if (GetKeyIfExistAndGotChildren(root, KEY_DATAPOINTS, outDP))
				{
					//DEBUG_PL(F("KDP:"));
					ProcessJsonDataPoints(outDP);
					retval = true;
				}
			}
			if ((filter & JsonProcessFilter::PROCESS_SCENES) == JsonProcessFilter::PROCESS_SCENES)
			{
				if (GetKeyIfExistAndGotChildren(root, KEY_SCENESTRIGGERED, outDP))
				{				
					//DEBUG_PL(F("SCT:"));
					//DEBUG_PL(recievedData);
					ProcessJsonSceneTrigger(outDP);
					retval = true;
				}
			}
			if ((filter & JsonProcessFilter::PROCESS_DEVICES) == JsonProcessFilter::PROCESS_DEVICES)
			{
				if (GetKeyIfExistAndGotChildren(root, KEY_DEVICES, outDP))
				{
					//DEBUG_P(F("DEV:"));
					if (ProcessJsonNewDevice(outDP, hexDeviceOut))
					{
						retval = true;
					}
				}
			}
		}
	}
	return retval;
}

bool FreeAtHomeESPapi::process()
{
	if (ws != NULL)
	{
		if ((!ws->isConnected()) && (ws->Available() <= 8))
		{
			//Disconnected and no data pending to process
			return false;
		}
		else
		{
			for (uint8_t i = 0; i < MAX_ESP_CREATED_DEVICES; i++)
			{
				if (EspDevices[i] != NULL)
				{
					EspDevices[i]->process();
				}
			}

			String msg = "";
			String totalmsg = "";
			unsigned int parts = 0;
			while (ws->getMessage(msg))
			{
				totalmsg += msg;
				parts++;
			}
			if (parts > 0)
			{
				ProcessJsonData(totalmsg, JsonProcessFilter::PROCESS_ACTIONS, NULL);
			}
			return true;
		}		
	}
	return false;
}

bool FreeAtHomeESPapi::GetStringToken(String& from, String& to, uint8_t index, char separator)
{
	uint16_t start = 0, idx = 0;
	uint8_t cur = 0;
	while (idx < from.length())
	{
		if (from.charAt(idx) == separator)
		{
			if (cur == index)
			{
				to = from.substring(start, idx);
				return true;
			}
			cur++;
			while ((idx < from.length() - 1) && (from.charAt(idx + 1) == separator)) idx++;
			start = idx + 1;
		}
		idx++;
	}
	if ((cur == index) && (start < from.length()))
	{
		to = from.substring(start, from.length());
		return true;
	}
	return false;
}

bool FreeAtHomeESPapi::ProcessJsonNewDevice(JsonObject& jsonDevices, uint64_t* hexDeviceOut)
{
	bool ret = false;
	JsonObject deviceProperties;

	if (hexDeviceOut != NULL)
	{
		if (jsonDevices.size() == 1)
		{
			for (JsonPair device : jsonDevices)
			{
				String strDevice = String(device.key().c_str());
				*hexDeviceOut = StringDevToU64(strDevice);
				ret = true;
				/*
				* //Currently device serial is not read, currently only used for processing results of local new device creation.
				deviceProperties = device.value().as<JsonObject>();
				//DEBUG_P("Device:"); DEBUG_PL(device.key().c_str());

				if (deviceProperties.containsKey(KEY_SERIAL))
				{
					const char* value = deviceProperties[KEY_SERIAL].as<JsonString>().c_str();
					//DEBUG_P("Value: ");
					//DEBUG_P(value);
					//NotifyCallbacks(FAHESPAPI_EVENT::FAHESPAPI_ON_NOTIFY_DEVICE_ID, hexDeviceOut, NULL, NULL, (void*)value);
				}
				*/
			}
		}
		/*else
		{
			DEBUG_P(F("Unexpected device count in new device processing: "));
			DEBUG_PL(jsonDevices.size());
		}*/
	}
	return ret;
}

String FreeAtHomeESPapi::ConstructDeviceRegistrationURI(const String& SerialNr)
{
	String URI = String(F("/fhapi/v1/api/rest/virtualdevice/")) + KEY_ROOT + String(F("/")) + SerialNr;
	return URI;
}

String FreeAtHomeESPapi::ConstructDeviceDataPointNotificationURI(const String& deviceFaHID, const String& channel, const String& datapoint)
{
	String URI = ConstructDeviceDataPointNotificationURI(deviceFaHID + String(F(".")) + channel + String(F(".")) + datapoint);
	return URI;
}

String FreeAtHomeESPapi::ConstructDeviceDataPointNotificationURI(const String& fulldatapoint)
{
	String URI = String(F("/fhapi/v1/api/rest/datapoint/")) + KEY_ROOT + String(F("/")) + fulldatapoint;
	return URI;
}

String FreeAtHomeESPapi::ConstructGetDeviceDetailsURI(const String& deviceFaHID)
{
	String URI = String(F("/fhapi/v1/api/rest/device/")) + KEY_ROOT + String(F("/")) + deviceFaHID;	
	return URI;
}

String FreeAtHomeESPapi::ConstructDeviceRegistrationBody(const String& DeviceType, const String &DisplayName, const uint16_t& Timeout)
{
	String jsonStringTimeout = String(F("\"ttl\":\"")) + String(Timeout) + String(F("\""));
	String jsonStringDisplayName = "";
	if (DisplayName.length() > 0)
	{
		jsonStringDisplayName = String(F(",\"displayname\":\"")) + DisplayName + String(F("\""));
	}
	String HTTPPostData = String(F("{\"type\":\"")) + DeviceType + String(F("\",\"properties\":{")) + jsonStringTimeout + jsonStringDisplayName + String(F("}}"));
	return HTTPPostData;
}


void FreeAtHomeESPapi::ProcessJsonDataPoints(JsonObject& jsonDataPoints)
{
	for (JsonPair kv : jsonDataPoints)
	{
		String strDataPoint = kv.key().c_str();
		String strDataPointPart1;
		String strDataPointPart2;

		const char* lpCchannel;
		const char* lpCDataPoint;
		const char* lpCValue;

		uint8_t token_idx = 0;
		if (GetStringToken(strDataPoint, strDataPointPart1, token_idx, KEY_DATAPOINT_SEPERATOR))
		{
			token_idx++;
			//DEBUG_P("PDP Dev: "); DEBUG_PL(strDataPointPart);
			uint64_t hexDevice = StringDevToU64(strDataPointPart1);
			if (isCallbackNeededForHexDevice(hexDevice))
			{
				if (GetStringToken(strDataPoint, strDataPointPart1, token_idx, KEY_DATAPOINT_SEPERATOR))
				{
					token_idx++;
					lpCchannel = strDataPointPart1.c_str();
					//DEBUG_PL(lpCchannel);

					if (GetStringToken(strDataPoint, strDataPointPart2, token_idx, KEY_DATAPOINT_SEPERATOR))
					{
						lpCDataPoint = strDataPointPart2.c_str();
						lpCValue = kv.value().as<const char*>();
						//DEBUG_PL(lpCDataPoint);	DEBUG_P("="); DEBUG_PL(kv.value().as<const char*>());
						NotifyCallbacks(FAHESPAPI_EVENT::FAHESPAPI_ON_DATAPOINT, hexDevice, lpCchannel, lpCDataPoint, (void*)lpCValue, false);

					}
				}
			}
		}
	}
}

void FreeAtHomeESPapi::ProcessjsonDataPointValueEntry(JsonObject& jsonDataPoint, uint64_t& hexDevice, JsonString& channel, const bool& isSceneOrGetValue)
{
	JsonObject valueobject;
	JsonString value;

	for (JsonPair datapoint : jsonDataPoint)
	{
		//DEBUG_P("   Datapoint:"); DEBUG_P(datapoint.key().c_str());	DEBUG_P(" = ");
		valueobject = datapoint.value().as<JsonObject>();
		if (valueobject.containsKey(KEY_VALUE))
		{
			value = valueobject[KEY_VALUE].as<JsonString>();
			//DEBUG_PL(value.c_str());
			const char* lpCchannel = channel.c_str();
			const char* lpCDataPoint = datapoint.key().c_str();
			const char* lpCValue = value.c_str();
			NotifyCallbacks(FAHESPAPI_EVENT::FAHESPAPI_ON_DATAPOINT, hexDevice, lpCchannel, lpCDataPoint, (void*)lpCValue, isSceneOrGetValue);
		}
	}
}

bool FreeAtHomeESPapi::isNightForSysAp()
{
	return bNightActuatorForSysAp;
}

bool FreeAtHomeESPapi::MatchChannelDataPoint(const char* ptrChannel, const char* ptrDataPoint, const uint8_t& Channel, const uint8_t& Datapoint, const bool &isInputDataPoint)
{
	if (ptrChannel == NULL || ptrDataPoint == NULL)
		return false;

	if(isInputDataPoint)
		return ((strcmp(ptrChannel, GetChannelString(Channel).c_str()) == 0) && (strcmp(ptrDataPoint, GetIDPString(Datapoint).c_str()) == 0));
	else
		return ((strcmp(ptrChannel, GetChannelString(Channel).c_str()) == 0) && (strcmp(ptrDataPoint, GetODPString(Datapoint).c_str()) == 0));
}

void FreeAtHomeESPapi::NotifyCallbacks(FAHESPAPI_EVENT Event, uint64_t FAHID, const char* ptrChannel, const char* ptrDataPoint, void* ptrValue, const bool& isSceneOrGetValue)
{
	if (Event == FAHESPAPI_EVENT::FAHESPAPI_ON_DATAPOINT)
	{
		if (SYSAP_FAH_ID == FAHID)
		{
			if(MatchChannelDataPoint(ptrChannel, ptrDataPoint, 0, 0, false))
			{
				bNightActuatorForSysAp = (String((const char*)ptrValue) == "1");
				/*for (uint8_t i = 0; i < 10; i++)
				{
					if (EspDevices[i] != NULL)
					{
						EspDevices[i]->DayNightToggle(isNight);
					}
				}*/
				return;
			}
		}

		for (uint8_t i = 0; i < 10; i++)
		{
			if (EspDevices[i] != NULL)
			{
				if (EspDevices[i]->GetFahDeviceID() == FAHID)
				{
					EspDevices[i]->NotifyFahDataPoint(String(ptrChannel), String(ptrDataPoint), String((const char*)ptrValue), isSceneOrGetValue);
				}
			}
		}
	}
	NotifyCallback(Event, FAHID, ptrChannel, ptrDataPoint, ptrValue);
}

bool FreeAtHomeESPapi::isCallbackNeededForHexDevice(uint64_t hexDevice)
{
	if (hexDevice == SYSAP_FAH_ID)
		return true;

	for (uint8_t i = 0; i < MAX_ESP_CREATED_DEVICES; i++)
	{
		if (EspDevices[i] != NULL)
		{
			if (EspDevices[i]->GetFahDeviceID() == hexDevice)
			{
				return true;
			}
		}
	}

	bool needsInfo = false;
	for (uint8_t i = 0; i < callbackcount; i++)
	{
		callbacks[i](FAHESPAPI_EVENT::FAHESPAPI_NEED_INFO, hexDevice, NULL, NULL, &needsInfo);
		if (needsInfo) //If at least one subscriber is interested in this device, process the trigger
			break;
	}
	return needsInfo;
}

void FreeAtHomeESPapi::ProcessJsonSceneTrigger(JsonObject& jsonSceneTrigger)
{
	JsonObject channels;
	JsonObject datapoints;

	for (JsonPair device : jsonSceneTrigger)
	{
		String strDevice = String(device.key().c_str());
		uint64_t hexDevice = StringDevToU64(strDevice);
		if (isCallbackNeededForHexDevice(hexDevice))
		{
			//DEBUG_P("Device:"); DEBUG_PL(device.key().c_str());
			if (GetNestedJsonObject(device, channels, KEY_CHANNELS))
			{
				for (JsonPair channel : channels)
				{
					JsonString strChannel = channel.key();
					//DEBUG_P(" Chan:");	DEBUG_PL(strChannel.c_str());

					if (GetNestedJsonObject(channel, datapoints, KEY_OUTPUTS))
					{
						//DEBUG_PL("inputs");
						ProcessjsonDataPointValueEntry(datapoints, hexDevice, strChannel, true);
					}
					if (GetNestedJsonObject(channel, datapoints, KEY_INPUTS))
					{
						//DEBUG_PL("outputs");
						ProcessjsonDataPointValueEntry(datapoints, hexDevice, strChannel, true);
					}
				}
			}
		}
	}
}

bool FreeAtHomeESPapi::GetNestedJsonObject(JsonPair& input, JsonObject& output, const String& ExpectedSubKey)
{
	JsonObject ref1 = input.value().as<JsonObject>();
	return GetKeyIfExistAndGotChildren(ref1, ExpectedSubKey, output);
}

bool FreeAtHomeESPapi::GetKeyIfExistAndGotChildren(JsonObject& rootObject, const String& LookFor, JsonObject& childRef)
{
	if (rootObject.containsKey(LookFor))
	{
		JsonObject __tchildRef = rootObject[LookFor].as<JsonObject>();
		if (__tchildRef.size() == 0)
			return false;

		childRef = __tchildRef;
		//DEBUG_P(F("GotChild: ")); DEBUG_P(KEY_DATAPOINTS); DEBUG_P(F(", count: ")); DEBUG_PL(__tchildRef.size());
		return true;
	}
	else
		return false;
}