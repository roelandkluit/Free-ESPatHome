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

//#define DEBUG
//#define FORCE_ESP8266_SSL_OPTION_AVAILBLE
#define MAX_ESP_CREATED_DEVICES 10
#define WS_PING_INTERVAL_TIMEOUT	  20
#define MAX_ARDUINOJSON_DOC_SIZE 10000


//Shared function defines

#define Seconds() (millis()/1000)

#ifdef DEBUG
#define DEBUG_PL Serial.println
#define DEBUG_P Serial.print
#else
#define DEBUG_PL(MSG)
#define DEBUG_P(MSG)
#endif
