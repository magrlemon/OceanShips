#pragma once

#include <string>
class NetWork
{
	NetWork(){}
	~NetWork() {}
public:
	static void InitUdp(std::string remoteip, unsigned short port);
	static void DestoryUdp();
	static void SendMsg(std::string);
	static std::string RecvFrom();
};