/*************************************************************************************************************
*
* Title			    : Free-ESPatHome
* Description:      : Library that implements the Busch-Jeager / ABB Free@Home API for ESP8266 and ESP32.
* Version		    : v 0.3
* Last updated      : 2023.10.23
* Target		    : ESP32, ESP8266, ESP8285
* Author            : Roeland Kluit
* Web               : https://github.com/roelandkluit/Free-ESPatHome
* License           : GPL-3.0 license
*
**************************************************************************************************************/
#pragma once
class FahSysAPInfo
{
	public:
		String Hostname = "";
		bool secure = false;
		uint16_t port = 80;
		String authorizationHeader = "";
};
