#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdint>
#include <random>
#include "ARINCWord.h"
#include "States.h"
#include "CommandWord.h"
#include "StatusWord.h"
#include "AircraftState.h"
#include <thread>
#include <chrono>
#include <mutex>


class MissionComputer {
public:
	MissionComputer();
	~MissionComputer();

	void Run();

private:
	bool InitializeSocket();
	void CloseSocket();

	void ReceiveARINCWord();
	bool DecodeWord();
	void UpdateState();

	void SendToDisplay();
	void SendToRT1();
	void SendToRT2();
	void SendToRT3();
	void SendToRT4();

	void Send1553Commands(uint8_t rtAddress);
	void Send1553Data(DecodedCommand decodedcmd);
	void Send1553Message(uint8_t rtAddress,
		uint8_t subAddress,
		const int16_t* dataWords,
		uint8_t wordCount);
	void Receive1553Data();
	DecodedStatus ReceiveStatus();

	void Bus1553Scheduler();
	void ARINCReceiverLoop();

private:
	SOCKET sockARINC = INVALID_SOCKET;
	SOCKET sock1553 = INVALID_SOCKET;
	sockaddr_in ARINCaddress{};
	sockaddr_in BUSaddress{};
	sockaddr_in displayDestination{};
	sockaddr_in busDestination{};
	sockaddr_in BUS{};
	sockaddr_in sender{};

	uint32_t raw;
	uint16_t data;
	ARINCWord word;

	Packet1553 cmdPacket;
	Packet1553 dataPacket;
	Packet1553 statusPacket;


	AircraftState aircraftState;
	DecodedCommand decodedcmd;

	std::mt19937 generator{ std::random_device{}() };
	std::uniform_int_distribution<int> addressroll{ 1, 1000 };

	std::chrono::steady_clock::time_point lastRT1Time;
	std::chrono::steady_clock::time_point lastRT2Time;
	std::chrono::steady_clock::time_point lastRT3Time;
	std::chrono::steady_clock::time_point lastRT4Time;

	std::mutex stateMutex;

	bool isRunning = false;
};