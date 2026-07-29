#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdint>
#include <random>
#include "ARINCWord.h"
#include "States.h"

class GPS {
public:
	GPS();
	~GPS();

	void Run();

private:
	bool InitializeSocket();
	void CloseSocket();
	void SendARINCWord(uint32_t raw);

	void UpdateGPSAltitude();
	void UpdateGroundSpeed();
	void UpdateLatitude();
	void UpdateLongitude();


private:
	SOCKET sock;
	sockaddr_in address{};
	sockaddr_in destination{};

	GPSState gpsState;

	ARINCWord gpsWord;

	std::mt19937 generator{ std::random_device{}() };

	std::uniform_int_distribution<int> altitudeDistribution{-20, 20};
	std::uniform_int_distribution<int> groundSpeedDistribution{-50, 50};
	std::uniform_int_distribution<int> latitudeDistribution{-3, 3};
	std::uniform_int_distribution<int> longitudeDistribution{-2, 2};


};