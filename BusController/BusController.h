#pragma once

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

class BusController {
public:
	BusController();
	~BusController();

	void Run();

private:
	void InitializeSocket();

	void SendCommand();
	void SendData();

	void ReceiveData();
	void ReceiveStatus();

private:
	SOCKET sock = INVALID_SOCKET;
	sockaddr_in address{};
	sockaddr_in destination{};
	sockaddr_in sender{};
	int destinationSize = sizeof(destination);
	int senderSize = sizeof(sender);
};