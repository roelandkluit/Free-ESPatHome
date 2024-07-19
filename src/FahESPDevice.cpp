/*************************************************************************************************************
*
* Title			    : Free-ESPatHome
* Description:      : Library that implements the Busch-Jeager / ABB Free@Home API for ESP8266 and ESP32.
* Version		    : v 0.12
* Last updated      : 2023.12.13
* Target		    : ESP32, ESP8266, ESP8285
* Author            : Roeland Kluit
* Web               : https://github.com/roelandkluit/Free-ESPatHome
* License           : GPL-3.0 license
*
**************************************************************************************************************/
#include "FahESPDevice.h"
#include "FreeAtHomeESPapi.h"

bool FahESPDevice::EnqueDataPoint(const bool& GetValue, const String Channel, const String DataPoint, const String Value)
{
	String reqType = String(F("GET"));
	if (!GetValue)
		reqType = String(F("PUT"));

	String sDeviceID = GetDeviceIDAsString();
	String Entry = reqType + ":" + sDeviceID + "." + Channel + "." + DataPoint + ":" + Value;
	//DEBUG_PL(Entry.c_str());
	return EnqueSetDataPoint(Entry);
}

bool FahESPDevice::EnqueSetDataPoint(const String Channel, const String DataPoint, const String Value)
{
	return EnqueDataPoint(false, Channel, DataPoint, Value);
}

bool FahESPDevice::EnqueGetDataPoint(const String Channel, const String DataPoint)
{
	return EnqueDataPoint(true, Channel, DataPoint, "");
}

void FahESPDevice::NotifyOnSysAPReconnect()
{	
}

bool FahESPDevice::EnqueSetDataPoint(const String Entry)
{
	if (PendingDataPointsCount == MAX_PENDING_DATAPOINTS)
		return false;

	PendingDataPointsCount++;

	String* newStrArray = new String[PendingDataPointsCount];
	for (uint8_t i = 0; i < PendingDataPointsCount - 1; i++)
	{
		newStrArray[i] = PendingDataPoints[i]; //Copy all items in the new array
	}
	newStrArray[PendingDataPointsCount - 1] = Entry; //Append new item to the end

	if (PendingDataPoints != NULL)
		delete[] PendingDataPoints;

	PendingDataPoints = newStrArray;
	return true;
}

String FahESPDevice::DequeDataPoint()
{
	if (PendingDataPointsCount == 0)
		return "";

	PendingDataPointsCount--;

	String* newStrArray = new String[PendingDataPointsCount];
	String returnStr = "" + PendingDataPoints[0]; //Get first item from list

	for (uint8_t i = 0; i < PendingDataPointsCount; i++)
	{
		newStrArray[i] = PendingDataPoints[i + 1]; //Move all items 1 up in the new array
	}	

	delete[] PendingDataPoints;
	PendingDataPoints = newStrArray;
	return returnStr;
}

void FahESPDevice::NotifyFahDataPoint(const String& strChannel, const String& strDataPoint, const String& strValue, const bool& isSceneOrGetValue)
{
}

uint64_t FahESPDevice::GetFahDeviceID()
{
	return FahDevice;
}

FahESPDevice::FahESPDevice(const String& FahDeviceType, const uint64_t& FahAbbID, const String& SerialNr, const uint16_t& timeout, FreeAtHomeESPapi* fahParent, FahSysAPInfo* SysApInfo)
{
	this->httpclt = new FahHTTPClient(SysApInfo);
	this->SysApApi = fahParent;
	this->FahDevice = FahAbbID;
	this->TimeOut = timeout;
	this->FahDeviceType = FahDeviceType;
	this->SerialNr = SerialNr;
	this->LastWaitInterval = millis();
	this->WaitTimeMs = (timeout * 3000) / 4;
}

FahESPDevice::~FahESPDevice()
{
	if (httpclt != NULL)
	{
		delete httpclt;
		httpclt = NULL;
	}
}

String FahESPDevice::ProcessJsonFromResponse(const String &Response)
{
	DynamicJsonDocument fahJsonMsg(3000);
	DeserializationError error = deserializeJson(fahJsonMsg, Response);

	if (error)
	{
		// Test if parsing succeeds.
		DEBUG_P(F("deserializeJson() failed: ")); DEBUG_PL(error.f_str());
		//DEBUG_PL(Response);
		return "";
	}
	else
	{
		if (fahJsonMsg.containsKey(FreeAtHomeESPapi::KEY_ROOT))
		{
			JsonObject root = fahJsonMsg[FreeAtHomeESPapi::KEY_ROOT].as<JsonObject>();
			//Is device value response
			if (root.containsKey(FreeAtHomeESPapi::KEY_VALUES))
			{
				JsonArray values = root[FreeAtHomeESPapi::KEY_VALUES].as<JsonArray>();
				if (values.size() == 1)
				{
					String s = values[0].as<String>();
					//DEBUG_P(s.c_str());
					return s;
				}
			}
			//Is device config response
			else if (root.containsKey(FreeAtHomeESPapi::KEY_DEVICES))
			{
				JsonObject devices = root[FreeAtHomeESPapi::KEY_DEVICES].as<JsonObject>();
				String DevString = GetDeviceIDAsString();
				if (devices.containsKey(DevString))
				{
					//Process Device and look for deviceName
					JsonObject device = devices[DevString].as<JsonObject>();
					if (device.containsKey(FreeAtHomeESPapi::KEY_DISPLAYNAME))
					{
						JsonString value = device[FreeAtHomeESPapi::KEY_DISPLAYNAME].as<JsonString>();
						DisplayName = "" + String(value.c_str());
						this->NotifyCallback(FAHESPAPI_EVENT::FAHESPAPI_ON_DISPLAYNAME, this->FahDevice, "", "", (void*)value.c_str());
					}

					//Process Channels and look for Parameters
					if (device.containsKey(FreeAtHomeESPapi::KEY_CHANNELS))
					{
						//DEBUG_PL("Channels");
						JsonObject channels = device[FreeAtHomeESPapi::KEY_CHANNELS].as<JsonObject>();
						for (JsonPair keyChan : channels)
						{
							String ChannelName = String(keyChan.key().c_str());
							JsonObject Chan = keyChan.value().as<JsonObject>();
							ProcessJsonDeviceParms(Chan, ChannelName);
						}
					}
					
					//Process Device and look for device parameters
					ProcessJsonDeviceParms(device, "");
				}				
			}
		}
	}
	return "";
}

void FahESPDevice::ProcessJsonDeviceParms(JsonObject &jsonObj, const String &channel)
{
	if (jsonObj.containsKey(FreeAtHomeESPapi::KEY_PARAMETERS))
	{
		//DEBUG_PL("PARMS!");
		JsonObject parms = jsonObj[FreeAtHomeESPapi::KEY_PARAMETERS].as<JsonObject>();
		for (JsonPair keyParm : parms)
		{
			String Parm = String(keyParm.key().c_str());
			if (Parm.indexOf(F("par")) == 0)
			{
				Parm = Parm.substring(3);
				uint16_t iParm = strtol(Parm.c_str(), NULL, 16);
				String Value = String(keyParm.value().as<JsonString>().c_str());
				NotifyDeviceParameter(channel, iParm, Value);
				/*Serial.print(channel);
				Serial.print("->");
				Serial.print();
				Serial.print("==");
				Serial.println(keyParm.value().as<JsonString>().c_str());*/
			}
		}
	}
}

String FahESPDevice::GetDeviceIDAsString()
{
	String sDeviceID = FreeAtHomeESPapi::U64toString(FahDevice);
	return sDeviceID;
}

void FahESPDevice::NotifyDeviceParameter(const String& strChannel, const uint16_t& Parameter, const String& strValue)
{
}

bool FahESPDevice::GetDataPointAndChannelFromURL(const String& URL, String& channel, String& datapoint)
{
	uint16_t end_pos = 0;
	uint16_t start_pos = 0;

	String sDeviceID = GetDeviceIDAsString() + ".";
	start_pos = URL.indexOf(sDeviceID);

	if (start_pos > 0)
	{
		start_pos = start_pos + sDeviceID.length(); //For the starting '[devicename].' before the channel
		end_pos = URL.indexOf('.', start_pos + 1);
		if (end_pos > 0)
		{
			channel = URL.substring(start_pos, end_pos);
			datapoint = URL.substring(end_pos + 1);
			return true;
		}
	}
	return false;
}

void FahESPDevice::processBase()
{
	if (httpclt->GetAsyncStatus() == HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_PENDING)
	{
		if ((millis() - httpclt->GetSessionStartMillis()) > HTTP_SESSION_TIMEOUT_MS)
		{
			httpclt->ReleaseAsync();
			lastSendGap = 200;
			if (LastDequedDataPoint != "")
			{
				EnqueSetDataPoint(LastDequedDataPoint);
			}
			else
			{
				LastDequedDataPoint = "";
			}
		}
		else
		{
			httpclt->ProcessAsync();
			return;
		}
	}
	else if (httpclt->GetAsyncStatus() == HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_FAILED)
	{
		//DEBUG_PL(httpclt->)
		httpclt->ReleaseAsync();
		lastSendGap = 200;
	}
	else if (httpclt->GetAsyncStatus() == HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_SUCCESS)
	{
		if (httpclt->LastRequestMethod() == String(F("GET")))
		{
			if (httpclt->GetResponseHeaderByKey(String(F("content-type"))) == String(F("application/json")))
			{
				if (httpclt->LastURIRequested().indexOf(F("/rest/datapoint/")) > 0)
				{
					String returndata = httpclt->GetBody();
					//DEBUG_P(String(F("ProcDP: ")));	DEBUG_PL(returndata);

					String Channel = "";
					String DataPoint = "";
					if (GetDataPointAndChannelFromURL(httpclt->LastURIRequested(), Channel, DataPoint))
					{
						String value = ProcessJsonFromResponse(returndata);
						if (value.length() > 0)
						{
							//DEBUG_F("CH: %s, DP: %s, Val: %s\r\n", Channel.c_str(), DataPoint.c_str(), value.c_str());
							NotifyFahDataPoint(Channel, DataPoint, value, true);
						}
					}
				}
				else if (httpclt->LastURIRequested().indexOf(F("/rest/device/")) > 0)
				{
					String returndata = httpclt->GetBody();
					//DEBUG_P(String(F("ProcRD: "))); DEBUG_PL(returndata);
					String d = ProcessJsonFromResponse(returndata);
				}
			}
		}
		httpclt->ReleaseAsync();
		lastSendGap = 50;
	}

	//Update Registration
	else if ((millis() - this->LastWaitInterval) > this->WaitTimeMs)
	{
		if (lastSendGap == 0)
		{
			//DEBUG_P(F("Update: ")); DEBUG_PL((millis() - this->LastWaitInterval));

			String URI = FreeAtHomeESPapi::ConstructDeviceRegistrationURI(SerialNr);
			String HTTPPostData = FreeAtHomeESPapi::ConstructDeviceRegistrationBody(this->FahDeviceType, "", this->TimeOut);
			if (httpclt->HTTPRequestAsync(String(F("PUT")), URI, HTTPPostData))
			{
				//ASync started, reset counter
				this->LastWaitInterval = millis();
				if (this->requestConfigSkip > 0)
				{
					this->requestConfigSkip--;
				}
			}
		}
	}

	//Send datapoints
	else if (PendingDataPointsCount > 0)
	{
		if (lastSendGap == 0)
		{
			LastDequedDataPoint = DequeDataPoint();
			String strDataPointRequestType = "";
			String strDataPointPart = "";
			String strDataPointValue = "";

			uint8_t token_idx = 0;
			if (FreeAtHomeESPapi::GetStringToken(LastDequedDataPoint, strDataPointRequestType, token_idx, ':'))
			{
				token_idx++;
				if (FreeAtHomeESPapi::GetStringToken(LastDequedDataPoint, strDataPointPart, token_idx, ':'))
				{
					token_idx++;
					if (strDataPointRequestType == String(F("GET")) || FreeAtHomeESPapi::GetStringToken(LastDequedDataPoint, strDataPointValue, token_idx, ':'))
					{
						String URI = FreeAtHomeESPapi::ConstructDeviceDataPointNotificationURI(strDataPointPart);
						//DEBUG_PL(URI); DEBUG_PL(strDataPointRequestType); DEBUG_PL(strDataPointPart);

						if (!httpclt->HTTPRequestAsync(strDataPointRequestType, URI, strDataPointValue))
						{
							if (PendingDataPointsCount != MAX_PENDING_DATAPOINTS)
							{
								//Put it back, failed to send
								//But only if the que is not filled completly
								EnqueSetDataPoint(LastDequedDataPoint);
							}
							lastSendGap = 200;
							//DEBUG_P("ERROR_DPN:"); DEBUG_P(strDataPointPart); DEBUG_P(", Value:"); DEBUG_PL(strDataPointValue);
						}
					}
				}
			}
		}
	}

	//Get Parameters
	else if (this->requestConfigSkip == 0 && lastSendGap == 0)
	{
		String URI = FreeAtHomeESPapi::ConstructGetDeviceDetailsURI(GetDeviceIDAsString());
		//DEBUG_PL(URI);

		if (!httpclt->HTTPRequestAsync("GET", URI, ""))
		{
			requestConfigSkip = 1;
		}
		else
		{
			requestConfigSkip = PARAMETER_REFRESH_INTERVAL;
		}		
	}
	if (lastSendGap > 0)
	{
		lastSendGap--;
	}
}

void FahESPDevice::process()
{	
	processBase();
}
