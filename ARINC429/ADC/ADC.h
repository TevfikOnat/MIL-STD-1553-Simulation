#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdint>
#include <random>
#include "ARINCWord.h"
#include "States.h"

class ADC {
public:
	ADC();
	~ADC();

	void Run();

private:
	bool InitializeSocket();
	void CloseSocket();
	void SendARINCWord(uint32_t raw);


	void UpdateBaroAltitude();
	void UpdateAirspeed();



private:
	SOCKET sock;
	sockaddr_in address{};
	sockaddr_in destination{};

	ADCState adcState;

	ARINCWord adcWord;

	std::mt19937 generator{ std::random_device{}() };

	std::uniform_int_distribution<int> baroaltitudeDistribution{ -20, 20 };
	std::uniform_int_distribution<int> airspeedDistribution{ -50, 50 };


};