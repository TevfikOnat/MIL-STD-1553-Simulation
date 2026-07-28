#pragma once

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")


class BUS {
public:
	BUS();
	~BUS();

	void Run();

private:
	void InitializeSocket();

	void ReceivePacket();

	void ForwardToRT(const char* buffer, int bytesReceived);

	void ForwardToBC(const char* buffer, int bytesReceived);

private:
	SOCKET sock = INVALID_SOCKET;

    sockaddr_in address{};
	sockaddr_in rtSockets[5]{};
	sockaddr_in buscontroller{};
	sockaddr_in sender{};

	int senderSize = sizeof(sender);
};