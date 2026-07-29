#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdint>
#include <random>
#include "ARINCWord.h"
#include "States.h"

class Radio {
public:
	Radio();
	~Radio();

	void Run();

private:
	bool InitializeSocket();
	void CloseSocket();

	void SendARINCWord(uint32_t raw);

	void UpdateAltitude();

private:
	SOCKET sock;
	sockaddr_in address{};
	sockaddr_in destination{};

	RadioState radioState;

	ARINCWord radioWord;

	std::mt19937 generator{ std::random_device{}() };

	std::uniform_int_distribution<int> altitudeDistribution{ -100, 100 };

};