#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include "ARINCWord.h"
#include "States.h"
#include "AircraftState.h"

#pragma comment(lib, "ws2_32.lib")

class Display {
public:
	Display();
	~Display();

	void Run();

private:
	bool InitializeSocket();
	void CloseSocket();

	void ReceiveState();
	void Update();
	void SSMCheck(int32_t value, SSM ssm);
	void Print();


private:
	SOCKET sock = INVALID_SOCKET;
	sockaddr_in address{};
	sockaddr_in sender{};

	uint32_t raw;
	AircraftState aircraftstate;
	ARINCWord word;

	GPSState gpsState;
	ADCState adcState;
	IRSState irsState;
	RadioState radioState;


};