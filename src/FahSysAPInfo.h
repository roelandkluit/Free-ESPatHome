#pragma once
class FahSysAPInfo
{
	public:
		String Hostname = "";
		bool secure = false;
		uint16_t port = 80;
		String authorizationHeader = "";
};
