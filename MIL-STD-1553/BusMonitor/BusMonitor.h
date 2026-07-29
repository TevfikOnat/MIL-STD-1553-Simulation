#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <array>
#include "CommandWord.h"
#include "StatusWord.h"
#include "Ports.h"

#pragma comment(lib, "ws2_32.lib")

class BusMonitor {
public:
	BusMonitor();
	~BusMonitor();

	void Run();

private:
	void InitializeSocket();
	void CloseSocket(SOCKET sock);

	void ReceiveCommand();
	void ReceiveData();
	void ReceiveStatus();

private:
	SOCKET sock = INVALID_SOCKET;
	sockaddr_in address{};
	sockaddr_in sender{};

	int senderSize = sizeof(sender);
	DecodedCommand command;

	uint16_t CommandWord;
	uint16_t DataWord;
	uint16_t statusWord;

	bool messageerror = FALSE;
	bool busy = FALSE;

	uint8_t memorysize = 31;

};