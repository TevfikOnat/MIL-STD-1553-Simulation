#pragma once

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "StatusWord.h"
#include "CommandWord.h"

#pragma comment(lib, "ws2_32.lib")

class BusController {
public:
	BusController();
	~BusController();

	void Run();

private:
	void InitializeSocket();
	void CloseSocket(SOCKET sock);

	void SendCommand();
	void SendData(DecodedCommand decodedcmd);

	void ReceiveData();
	DecodedStatus ReceiveStatus();

private:
	SOCKET sock = INVALID_SOCKET;
	sockaddr_in address{};
	sockaddr_in destination{};
	sockaddr_in sender{};
	int destinationSize = sizeof(destination);
	int StatusReceived;
	DecodedCommand decodedcmd;
};