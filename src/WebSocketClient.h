#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <WiFiClient.h>
#include "FahESPBuildConfig.h"

class WebSocketClient {
public:

	WebSocketClient(bool secure = false);

	~WebSocketClient();

	bool connect(const String& host, const String& path, uint16_t port);

	bool isConnected();

	int Available();

	void disconnect();
	
	void send(const String& str);	

	bool getMessage(String& message);

	void setAuthorizationHeader(const String& header);

private:
	int timedRead();

	void sendKeepAlive();

	void send(const String& str, const uint8_t& wsOpcode);

    void write(uint8_t data);
    
    void write(const char *str);

	String generateKey();

	WiFiClient *client;

	String authorizationHeader = "";

    bool websocketEstablished = false;

	unsigned long LastWaitInterval = 0;

};

#endif //WEBSOCKETCLIENT_H
