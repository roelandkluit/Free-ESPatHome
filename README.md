# Free-ESPatHome

## Library that implements the Busch-Jeager / ABB Free@Home API for ESP8266 and ESP32.

It implements a basic connection to a Busch-Jeaer / ABB system access point. Including the websocket for events and the device and datapoint api for device creation.

A basic implementation of the Virtual Switch is included.

## Example usage
### Monitoring Events
Here is sample code showing how to use the `Free-ESPatHome` libary for monitoring FaH events: 

```
#include "FreeAtHomeESPapi.h"
FreeAtHomeESPapi freeAtHomeESPapi;

void FahCallBack(FAHESPAPI_EVENT Event, uint64_t FAHID, const char* ptrChannel, const char* ptrDataPoint, void* ptrValue)
{
	if (Event == FAHESPAPI_EVENT::FAHESPAPI_NEED_INFO)
	{
    //Enable the condition and set value if you only what to recieve events for a specific device:
		//if (FAHID == 0xABB700CE0ACC) 
		{
			bool* val = ((bool*)ptrValue);
			*val = true;
		}
	}
	else if (Event == FAHESPAPI_EVENT::FAHESPAPI_ON_DATAPOINT)
	{
		String t;
		freeAtHomeESPapi.U64toStringDev(FAHID, t);
    const char* vp = (const char*)ptrValue;

    String Text = "Device: " + t + ", Datapoint: " + ptrChannel + "." + ptrDataPoint + " = " + vp;
		Serial.println(Text);
	}
}

void setup(void)
{
  ...
  freeAtHomeESPapi.AddCallback(FahCallBack);
  ...
}

void loop(void)
{
  //Call the process often!
	if (!freeAtHomeESPapi.process())
	{
    //Connection lost or not connected
		Serial.println(F("Connecting WebSocket"));
		if(!freeAtHomeESPapi.ConnectToSysAP("sysAp", "sysApUserGuid", "sysApPassword", false))
		{
			Serial.println(F("Failed to connect to SysAp! Check Sysap hostname and account info!"));
			delay(5000);
		}
	}
}
```

### Creating a virtual Switch

```
#include "FreeAtHomeESPapi.h"
#include "FahESPDevice.h"
#include "FahESPSwitchDevice.h"

FreeAtHomeESPapi freeAtHomeESPapi;
FahESPSwitchDevice* espDev = NULL;

void FahCallBack(FAHESPAPI_EVENT Event, uint64_t FAHID, const char* ptrChannel, const char* ptrDataPoint, void* ptrValue)
{
	if (Event == FAHESPAPI_EVENT::FAHESPAPI_ON_DEVICE_EVENT)
	{
		String t;		
		bool val = ((bool)ptrValue);	
		freeAtHomeESPapi.U64toStringDev(FAHID, t);

		String Text = "Device: " + t + ", DeviceEvent: " + ptrChannel + "-" + ptrDataPoint + " = " + val;
		Serial.println(Text);		
	}
}

void setup(void)
{
  ...
}

void loop(void)
{
  //Call the process often!
	if (!freeAtHomeESPapi.process())
	{
    //Connection lost or not connected
		Serial.println(F("Connecting WebSocket"));
		if(!freeAtHomeESPapi.ConnectToSysAP("sysAp", "sysApUserGuid", "sysApPassword", false))
		{
			Serial.println(F("Failed to connect to SysAp! Check Sysap hostname and account info!"));
			delay(5000);
		}
	}
  else
	{	
		if (espDev == NULL)
		{
			Serial.println(F("Create Switch Device"));
			espDev = freeAtHomeESPapi.CreateSwitchDevice("TestSwitch", "Virtual Test Switch", 300);
			if (espDev != NULL)
			{
				espDev->AddCallback(FahCallBack);
			}
			else
			{
				Serial.println(F("Failed to create Virtual device, check user authorizations"));
				delay(5000);
			}
		}
	}
}
```

Currently only the VirtualSwitch device is implemented.

## License
GNU General Public License v3.0