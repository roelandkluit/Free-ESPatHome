//#define DEBUG

//Original obtained from: https://github.com/hellerchr/esp8266-websocketclient 
//Modified by Roeland Kluit
// Updated SSL
// Non blocking
// Added keepalives
// Added message continuation

#include "WebSocketClient.h"
#include <WiFiClientSecure.h>

#define WS_FIN            0x80
#define WS_OPCODE_TEXT    0x01
#define WS_OPCODE_BINARY  0x02
#define WS_OPCODE_PING    0x09
#define WS_OPCODE_PONG    0x0A


#define WS_MASK           0x80
#define WS_SIZE16         126

WebSocketClient::WebSocketClient(bool secure) {
	if (secure) {
		WiFiClientSecure *w = new WiFiClientSecure();
		w->setInsecure();  // Disable certificate verification
		this->client = w;
	}
	else
		this->client = new WiFiClient;
}

WebSocketClient::~WebSocketClient() {
	while (this->client->available())
	{
		//Empty buffers
		this->client->read();
	}
	delete this->client;
}

void WebSocketClient::setAuthorizationHeader(const String& header) {
	this->authorizationHeader = header;
}

String WebSocketClient::generateKey() {
	String key = "";
	for (int i = 0; i < 22; ++i) {
		int r = random(0, 3);
		if (r == 0)
			key += (char) random(48, 57);
		else if (r == 1)
			key += (char) random(65, 90);
		else if (r == 2)
			key += (char) random(97, 122);
	}
	return key;
}
void WebSocketClient::write(uint8_t data) {
    if (client->connected())
        client->write(data);
}

void WebSocketClient::write(const char *data) {
    if (client->connected())
        client->write(data);
}

bool WebSocketClient::connect(const String& host, const String& path, uint16_t port)
{
	if (isConnected())
	{
		//Already connected
		return true;
	}
	if (!client->connect(host.c_str(), port))
	{
		DEBUG_PL(String(F("No connection to host")));
		return false;
	}

	// send handshake
	String handshake = String(F("GET ")) + path + String(F(" HTTP/1.1\r\nHost: ")) + host + 
			String(F("\r\nConnection: Upgrade\r\nUpgrade: websocket\r\nSec-WebSocket-Version: 13\r\nSec-WebSocket-Key: ")) +
			generateKey() + String(F("==\r\n"));

	if (authorizationHeader != "")
		handshake += String(F("Authorization: ")) + authorizationHeader + String(F("\r\n"));

	handshake += String(F("\r\n"));

	DEBUG_P(F("[WS] sending handshake: "));
	DEBUG_PL(handshake);

    write(handshake.c_str());

	// success criteria
	bool hasCorrectStatus = false;
	bool isUpgrade = false;
	bool isWebsocket = false;
	bool hasAcceptedKey = false;

	bool endOfResponse = false;

	// handle response headers
	String s;
	while (!endOfResponse && (s = client->readStringUntil('\n')).length() > 0) {
		DEBUG_PL(String(F("[WS][RX] ")) + s);
		// HTTP Status
		if (s.indexOf(F("HTTP/")) != -1) {
			auto status = s.substring(9, 12);
			if (status == String(F("101")))
				hasCorrectStatus = true;
			else {
				DEBUG_PL(String(F("[WS] wrong status: ")) + status);
				return false;
			}
		}
		// Headers
		else if (s.indexOf(':') != -1) {
			auto col = s.indexOf(':');
			auto key = s.substring(0, col);
			key.toLowerCase();  // Make all headers lowercase for case-insensitve comparison
			auto value = s.substring(col + 2, s.length() - 1);

			if (key == String(F("connection")) && (value == String(F("Upgrade")) || value == String(F("upgrade"))))
				isUpgrade = true;

			else if (key == String(F("sec-websocket-accept")))
				hasAcceptedKey = true;

			else if (key == String(F("upgrade")) && value == String(F("websocket")))
				isWebsocket = true;
		}

		else if (s == "\r")
			endOfResponse = true;
	}

	bool success = hasCorrectStatus && isUpgrade && isWebsocket && hasAcceptedKey;

	if (success) {
		DEBUG_PL(F("[WS] sucessfully connected"));
        this->websocketEstablished = true;
		this->LastWaitInterval = millis();
    }
	else {
		DEBUG_PL(F("[WS] could not connect"));
        this->disconnect();
	}	

	return success;
}

bool WebSocketClient::isConnected() {
	return this->websocketEstablished && client->connected();
}

int WebSocketClient::Available()
{
	return client->available();
}

void WebSocketClient::disconnect() {
	client->stop();
    this->websocketEstablished = false;
}

void WebSocketClient::send(const String& str)
{
	send(str, WS_OPCODE_TEXT);
}

void WebSocketClient::send(const String& str, const uint8_t& wsOpcode) {
	//DEBUG_PL(String(F("[WS] sending: ")) + str);
	if (!client->connected()) {
		DEBUG_PL(F("[WS] not connected..."));
		return;
	}

	// 1. send fin and opcode
	write(WS_FIN | wsOpcode);

	// 2. send length
	int size = str.length();
	if (size > 125) {
		write(WS_MASK | WS_SIZE16);
		write((uint8_t) (size >> 8));
		write((uint8_t) (size & 0xFF));
	} else {
		write(WS_MASK | (uint8_t) size);
	}

	// 3. send mask
	uint8_t mask[4];
	mask[0] = random(0, 256);
	mask[1] = random(0, 256);
	mask[2] = random(0, 256);
	mask[3] = random(0, 256);

	write(mask[0]);
	write(mask[1]);
	write(mask[2]);
	write(mask[3]);

	//4. send masked data
	for (int i = 0; i < size; ++i) {
		write(str[i] ^ mask[i % 4]);
	}
}

int WebSocketClient::timedRead()
{
	//timedRead is currently blocking
	//Todo fix if data is not available for ~250 millis()
	while (!client->available())
	{
		if (!client->connected())
		{
			return 0;
		}
		delay(5);
	}
	return client->read();
}

void WebSocketClient::sendKeepAlive()
{
	if ((millis() - this->LastWaitInterval) > WS_PING_INTERVAL_TIMEOUT)
	{
		//DEBUG_PL(F("ping"));
		send(String(F("{}")), WS_OPCODE_PING);
		this->LastWaitInterval = millis();
	}
}

bool WebSocketClient::getMessage(String& message) {
	if (!client->connected())
	{
		if(client->available() < 8)
		{
			client->stop();
			return false;
		}
		else
		{
			//Disconnected, but there is still data in the pipeline
			//Continue to process this message
		}
	}

	if (!client->available())
	{
		sendKeepAlive();
		return false;
	}

	// 1. read type and fin
	unsigned int msgtype = timedRead();
	/*if (!client->connected())
	{
		return false;
	}*/

	// 2. read length and check if masked
	unsigned int length = timedRead();
	bool hasMask = false;
	if (length & WS_MASK) {
		hasMask = true;
		length = length & ~WS_MASK;
	}

	if (length == WS_SIZE16) {
		length = timedRead() << 8;
		length |= timedRead();
	}

	// 3. read mask
	if (hasMask) {
		uint8_t mask[4];
		mask[0] = timedRead();
		mask[1] = timedRead();
		mask[2] = timedRead();
		mask[3] = timedRead();

		// 4. read message (masked)
		//message = "";
		for (int i = 0; i < length; ++i) {
			message += (char) (timedRead() ^ mask[i % 4]);
		}
	} else {
		// 4. read message (unmasked)
		//message = "";
		for (int i = 0; i < length; ++i) {
			message += (char) timedRead();
		}
	}

	if ((msgtype & WS_OPCODE_PONG) != WS_OPCODE_PONG)
	{
		//DEBUG_P(String(F("Opcode: "))); DEBUG_PL(msgtype);

		if ((msgtype & WS_FIN) == WS_FIN)
		{
			//DEBUG_PL(String(F("MSG_done!")));
			//DEBUG_PL(message);
			//DEBUG_PL(String(F("END_done!")));
		}
		else
		{
			//DEBUG_PL(String(F("MSG_Moredata!")));
			getMessage(message);
			//DEBUG_PL(message);
			//DEBUG_PL(String(F("END_Moredata!")));
		}
	}

	if ((msgtype & WS_OPCODE_TEXT) == WS_OPCODE_TEXT)
	{
		return true;
	}
    return false;
}
