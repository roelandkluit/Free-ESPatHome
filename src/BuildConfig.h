#pragma once

//#define DEBUG
//#define FORCE_ESP8266_SSL_OPTION_AVAILBLE
#define MAX_ESP_CREATED_DEVICES 10
#define WS_PING_INTERVAL_TIMEOUT	  20
#define MAX_ARDUINOJSON_DOC_SIZE 8000


//Shared function defines

#define Seconds() (millis()/1000)

#ifdef DEBUG
#define DEBUG_PL Serial.println
#define DEBUG_P Serial.print
#else
#define DEBUG_PL(MSG)
#define DEBUG_P(MSG)
#endif
