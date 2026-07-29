#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdint>
#include <random>
#include "ARINCWord.h"
#include "States.h"


class IRS {
public:
	IRS();
	~IRS();

	void Run();

private:
	bool InitializeSocket();
	void CloseSocket();
	
	void SendARINCWord(uint32_t raw);

	void UpdateHeading();
	void UpdateRoll();
	void UpdatePitch();

private:
	SOCKET sock;
	sockaddr_in address{};
	sockaddr_in destination{};

	IRSState irsState;

	ARINCWord irsWord;

	std::mt19937 generator{ std::random_device{}() };

	std::uniform_int_distribution<int> headingDistribution{ 0, 1 };
	std::uniform_int_distribution<int> rollDistribution{ -1, 1 };
	std::uniform_int_distribution<int> pitchDistribution{ -5, 5 };		 
};