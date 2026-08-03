#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <array>
#include "CommandWord.h"
#include "StatusWord.h"
#include "Ports.h"
#include <iomanip>
#include <chrono>

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

	void ReceivePacket();
	void ReceiveCommand(uint16_t CommandWord);
	void ReceiveData(uint16_t DataWord);
	void ReceiveStatus(uint16_t StatusWord);
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

	Packet1553 packet;

	uint16_t CommandWord;
	uint16_t DataWord;
	uint16_t statusWord;

	uint32_t wordsSent = 0;
	uint32_t dataSent = 0;
	uint32_t statusSent = 0;
	uint32_t commandSent=0;

	bool messageerror = FALSE;
	bool busy = FALSE;

	uint8_t memorysize = 31;

	ErrorCount RTs[4];
	int Terminal;
	int TerminalAddress;

	std::chrono::steady_clock::time_point lastPrintTime;

};

