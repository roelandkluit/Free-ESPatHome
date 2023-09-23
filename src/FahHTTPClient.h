#pragma once
#include "HTTPClient.h"
#include "FahSysAPInfo.h"

namespace HTTPREQUEST_STATUSUS
{
	enum HTTPREQUEST_STATUS :uint8_t
	{
		HTTPREQUEST_STATUS_NONE = 0,
		HTTPREQUEST_STATUS_SUCCESS = 1,
		HTTPREQUEST_STATUS_FAILED = 2,
		HTTPREQUEST_STATUS_TIMEOUT = 3,
		HTTPREQUEST_STATUS_PENDING = 4,
	};
}
typedef HTTPREQUEST_STATUSUS::HTTPREQUEST_STATUS HTTPREQUEST_STATUS;

class FahHTTPClient : public HTTPClient
{
private:
	FahSysAPInfo* SysAp = NULL;
	bool ConnectToSysAp();
	bool PutHTTPRequest(const String& Method, const String& URI, const String& PostData);
	HTTPREQUEST_STATUS ProcessHTTPHeaders();
	HTTPREQUEST_STATUS ProcessHTTPBody();
	HTTPREQUEST_STATUS GetHTTPRequestResult();
	String Async_URI;
	String Async_PostData;
	String Async_Method;
	HTTPREQUEST_STATUS AsyncStatus = HTTPREQUEST_STATUS::HTTPREQUEST_STATUS_NONE;
public:
	HTTPREQUEST_STATUS GetAsyncStatus();
	FahHTTPClient(FahSysAPInfo* sysApInfo);
	bool HTTPRequestAsync(const String& Method, const String& URI, const String& PostData);
	void ProcessAsync();
	void ReleaseAsync();
	bool HTTPRequest(const String& URI, const String& Method, const String& PostData);
	~FahHTTPClient();
};