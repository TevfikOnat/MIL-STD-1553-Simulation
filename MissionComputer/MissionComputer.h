#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdint>
#include <random>
#include "ARINCWord.h"
#include "States.h"
#include "AircraftState.h"


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

private:
	SOCKET sock = INVALID_SOCKET;
	sockaddr_in address{};
	sockaddr_in destination{};
	sockaddr_in sender{};

	uint32_t raw;
	ARINCWord word;

	AircraftState aircraftState;
};