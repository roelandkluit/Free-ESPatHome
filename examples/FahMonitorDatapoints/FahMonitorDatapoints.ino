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

#ifndef STASSID
#define STASSID "WifiSSID"
#define STAPSK "WifiPassword"
#endif

//FaH System Accespoint IP or Hostname
const char* sysAp = "sysap";
//User ID GUID for sysAp, retrieve user GUIDs from http://sysap/swagger/users
const char* sysApUser = "6828fafd-e457-4812-821c-10cd6ccdd28b"; 
const char* sysApPassword = "Password"; //Password for user, 

const char* host = "esp-FahMonitor";
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

String Text = "No Datapoint recieved";
FreeAtHomeESPapi freeAtHomeESPapi;

void FahCallBack(FAHESPAPI_EVENT Event, uint64_t FAHID, const char* ptrChannel, const char* ptrDataPoint, void* ptrValue)
{
	if (Event == FAHESPAPI_EVENT::FAHESPAPI_NEED_INFO)
	{
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

    Text = "Device: " + t + ", Datapoint: " + ptrChannel + "." + ptrDataPoint + " = " + vp;
		Serial.println(Text);
	}
}

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
  freeAtHomeESPapi.AddCallback(FahCallBack);

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
}
