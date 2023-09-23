#define UseWebServer

#ifdef ESP32
#include <dummy.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <HTTPUpdateServer.h>
#else
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#ifdef UseWebServer
	#include <ESP8266WebServer.h>
	#include <ESP8266mDNS.h>
	#include <ESP8266HTTPUpdateServer.h>
#endif
#else
#error "Platform not supported"
#endif
#endif

#include "FreeAtHomeESPapi.h"
#include "FahESPDevice.h"
#include "FahESPSwitchDevice.h"

#ifndef STASSID
#define STASSID "WifiSID"
#define STAPSK "WifiPassword"
#endif

//FaH System Accespoint IP or Hostname
const char* sysAp = "sysap";
//User ID GUID for sysAp, retrieve user GUIDs from http://sysap/swagger/users
const char* sysApUser = "6828fafd-e457-4812-821c-10cd6ccdd28b"; 
const char* sysApPassword = "Password"; //Password for user, 

const char* host = "esp-FaHvSwitch";
const char* ssid = STASSID;
const char* password = STAPSK;

#ifdef ESP32
	#ifdef UseWebServer
		WebServer httpServer(80);
		HTTPUpdateServer httpUpdater;
	#endif
#endif
#ifdef ESP8266
	#ifdef UseWebServer
		ESP8266WebServer httpServer(80);
		ESP8266HTTPUpdateServer httpUpdater;
	#endif
#endif

String Text = "No Device Event Recieved";
FreeAtHomeESPapi freeAtHomeESPapi;
FahESPSwitchDevice* espDev = NULL;

void FahCallBack(FAHESPAPI_EVENT Event, uint64_t FAHID, const char* ptrChannel, const char* ptrDataPoint, void* ptrValue)
{
	if (Event == FAHESPAPI_EVENT::FAHESPAPI_ON_DEVICE_EVENT)
	{
		String t;		
		bool val = ((bool)ptrValue);	
		freeAtHomeESPapi.U64toStringDev(FAHID, t);

		Text = "Device: " + t + ", DeviceEvent: " + ptrChannel + "-" + ptrDataPoint + " = " + val;
		Serial.println(Text);		
	}
}

unsigned long boot_unixtimestamp = 0;

void handleRoot()
{
	httpServer.send(200, "text/plain", Text.c_str());
}

void setup(void)
{
  Serial.begin(115200);
  Serial.println();
  Serial.println(F("Booting Sketch..."));
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    Serial.println(F("WiFi failed, retrying."));
  }

  #ifdef UseWebServer
	MDNS.begin(host);
	MDNS.addService("http", "tcp", 80);

	httpUpdater.setup(&httpServer);
	httpServer.begin();
	httpServer.on("/", handleRoot);

	MDNS.addService("http", "tcp", 80);
	Serial.printf("HTTPUpdateServer ready! Open http://%s.local/update in your browser\n", host);
  #endif // UseWebServer

  Serial.print(F("FREE-ESP@HOME IP: "));
  Serial.println(WiFi.localIP());
}

void loop(void)
{
	#ifdef UseWebServer
		httpServer.handleClient();
		#ifdef ESP8266
			MDNS.update();
		#endif
	#endif
	if (!freeAtHomeESPapi.process())
	{
		Serial.println(F("Connecting WebSocket"));
		if(!freeAtHomeESPapi.ConnectToSysAP(sysAp, sysApUser, sysApPassword, false))
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
				String t;
				espDev->AddCallback(FahCallBack);
				freeAtHomeESPapi.U64toStringDev(espDev->GetFahDeviceID(), t);
				Serial.print(t);
				Serial.println(F(": Succes!"));
				Text = "Virtual Device Created";
			}
			else
			{
				Serial.println(F("Failed to create Virtual device, check user authorizations"));
				delay(5000);
			}
		}
	}
}
