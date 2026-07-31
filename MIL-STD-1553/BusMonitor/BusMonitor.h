#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <array>
#include "CommandWord.h"
#include "StatusWord.h"
#include "Ports.h"
#include <iomanip>

#pragma comment(lib, "ws2_32.lib")

struct ErrorCount
{
	int messageCount = 0;
	double busyCount = 0;
	double meCount = 0;
	double busyPercent = 0;
	double mePercent = 0;
};

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
	void ErrorLog();

	void Print();
	void ClearScreen();

private:
	SOCKET sock = INVALID_SOCKET;
	sockaddr_in address{};
	sockaddr_in sender{};

	int senderSize = sizeof(sender);
	DecodedCommand command;
	DecodedStatus decodedstat;

	uint16_t CommandWord;
	uint16_t DataWord;
	uint16_t statusWord;

	uint16_t wordsSent = 0;
	uint16_t dataSent = 0;
	uint16_t statusSent = 0;
	uint16_t commandSent=0;

	bool messageerror = FALSE;
	bool busy = FALSE;

	uint8_t memorysize = 31;

	ErrorCount RTs[4];
	int Terminal;
	int TerminalAddress;

};

