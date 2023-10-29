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

#ifdef ESP32
#else
	#ifdef ESP8266
	#else
		#error "Device not supported, FreeAtHomeESPapi requires ESP platform"
	#endif
#endif

#include "FahEventEnabledClass.h"
#include <ArduinoJson.h>
#include <stdint.h>
#include "WebSocketClient.h"
#include "HTTPClient.h"
#include "FahESPDevice.h"
#include "FahSysAPInfo.h"

class FahSysAPInfo;

//ESP Device Childs are defined as classes here, include is done in CPP!
class FahESPDevice;
class FahESPSwitchDevice;
class FahESPWeatherStation;

class FreeAtHomeESPapi : public FahEventEnabledClass
{
public:
	static const uint64_t SYSAP_FAH_ID = 0xABB700000000;
	static const uint8_t FAHESP_VERSION_MAJOR = 0;
	static const uint8_t FAHESP_VERSION_MINOR = 4;
	static String Version() { return String(FAHESP_VERSION_MAJOR) + "." + String(FAHESP_VERSION_MINOR) + String(F(" - Roeland Kluit")); }
	FreeAtHomeESPapi();
	~FreeAtHomeESPapi();
	bool ConnectToSysAP(const String& SysAPHostname, const String& AuthorizationHeader, const bool& useSSL);
	bool ConnectToSysAP(const String& SysAPHostname, const String& Username, const String& Password, const bool& useSSL);
	bool process();
	bool isNightForSysAp();
	static bool MatchChannelDataPoint(const char* ptrChannel, const char* ptrDataPoint, const uint8_t& Channel, const uint8_t& Datapoint, const bool& isInputDataPoint);
	static String GetIDPString(const uint8_t &Number);
	static String GetChannelString(const uint8_t &Number);
	static String GetODPString(const uint8_t &Number);	
	static uint64_t StringDevToU64(const String& DeviceID);
	static void U64toStringDev(const uint64_t number, String& stringref);
	static String U64toString(const uint64_t number);
	FahESPSwitchDevice* CreateSwitchDevice(const String& SerialNr, const String& DisplayName, const uint16_t& timeout);
	FahESPWeatherStation* CreateWeatherStation(const String& SerialNr, const String& DisplayName, const uint16_t& timeout);
	FahESPDevice* CreateDevice(const String& SerialNr, const String& deviceType, const String& DisplayName, const uint16_t& timeout);	
	static String ConstructDeviceRegistrationURI(const String& SerialNr);
	static String ConstructDeviceRegistrationBody(const String& DeviceType, const String& DisplayName, const uint16_t& Timeout);
	static String ConstructDeviceDataPointNotificationURI(const String& deviceFaHID, const String& channel, const String& datapoint);
	static String ConstructDeviceDataPointNotificationURI(const String& fulldatapoint);
	static bool GetStringToken(String& from, String& to, uint8_t index, char separator);
	bool RemoveDevice(FahESPDevice* Device);
	static const String KEY_ROOT;
	static const String KEY_DATAPOINTS;
	static const String KEY_CHANNELS;
	static const String KEY_OUTPUTS;
	static const String KEY_INPUTS;
	static const String KEY_VALUE;
	static const String KEY_DEVICES;
	static const String KEY_SCENESTRIGGERED;
	static const String VALUE_0;
	static const String VALUE_1;
	static const char KEY_DATAPOINT_SEPERATOR = '/';
private:
	enum httpClientSessionStatus:uint8_t
	{
		SESSION_NONE = 0,
		SESSION_CONNECTING,
		SESSION_CONNECTED,
		SESSION_CONNECT_FAILED,
		SESSION_REQUEST_SEND,
		SESSION_RESPONSE_HEADERS_OK,
		SESSION_RESPONSE_HEADERS_ERROR,		
		SESSION_TIMEOUT,
	};
	enum JsonProcessFilter :uint8_t
	{
		PROCESS_NONE = 0,
		PROCESS_DATAPOINTS = 1,
		PROCESS_SCENES = 2,
		PROCESS_DEVICES = 4,
		PROCESS_ACTIONS = PROCESS_DATAPOINTS | PROCESS_SCENES,
		PROCESS_ALL = PROCESS_ACTIONS | PROCESS_DEVICES,
	};
	FahSysAPInfo* SysApInfo = NULL;
	WebSocketClient* ws = NULL;
	FahESPDevice* EspDevices[MAX_ESP_CREATED_DEVICES] = { NULL };
	uint8_t EspDevicesCount;
	bool bNightActuatorForSysAp = false;
	bool RegisterFahEspDevice(FahESPDevice* Device);
	bool ProcessJsonData(String& recievedData, JsonProcessFilter filter, uint64_t* hexDeviceOut);	
	bool isCallbackNeededForHexDevice(uint64_t hexDevice);
	void ProcessjsonDataPointValueEntry(JsonObject& jsonDataPoint, uint64_t& hexDevice, JsonString& channel, const bool& isScene);
	bool GetKeyIfExistAndGotChildren(JsonObject& rootObject, const String& LookFor, JsonObject& childRef);
	bool GetNestedJsonObject(JsonPair& input, JsonObject& output, const String& ExpectedSubKey);
	void NotifyCallbacks(FAHESPAPI_EVENT Event, uint64_t FAHID, const char* ptrChannel, const char* ptrDataPoint, void* ptrValue, const bool& isScene);
	void ProcessJsonDataPoints(JsonObject& jsonDataPoints);
	void ProcessJsonSceneTrigger(JsonObject& jsonSceneTrigger);
	bool ProcessJsonNewDevice(JsonObject& jsonDevices, uint64_t* hexDeviceOut);
	static String GetPadString(const String& refString, const uint8_t &size, const char& padChar);
};
