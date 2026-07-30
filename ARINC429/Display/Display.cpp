#include "Display.h"
#include "Ports.h"
#include <iostream>

Display::Display() {
	if (!InitializeSocket())
	{
		throw std::runtime_error("Socket initialization failed");
	}
}

Display::~Display() {
	CloseSocket();
}

bool Display::InitializeSocket() {
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "WSAStartup failed" << std::endl;
        return false;
    }

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (sock == INVALID_SOCKET)
    {
        std::cout << "Invalid Socket" << std::endl;
        return false;
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(Ports::Display);
    address.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == SOCKET_ERROR) {
        std::cout << "Bind failed: " << WSAGetLastError() << std::endl;
        return false;
    }
    return true;
}

void Display::CloseSocket() {
	closesocket(sock);
	WSACleanup();
}  

void Display::ReceiveState() {
	int senderSize = sizeof(sender);
	int bytesReceived = recvfrom(sock, reinterpret_cast<char*>(&aircraftstate), sizeof(aircraftstate), 0,
		reinterpret_cast<sockaddr*>(&sender), &senderSize);
	if (bytesReceived == SOCKET_ERROR) {
		std::cout << "Receive failed: " << WSAGetLastError() << std::endl;
	}
}

void Display::SSMCheck(int32_t value, SSM ssm) {

	switch (ssm) {
	case SSM::NormalOperation:
		std::cout << value << std::endl;
		break;
	case SSM::FunctionalTest:
		std::cout << "TEST" << std::endl;
		break;
	case SSM::FailureWarning:
		std::cout << "FAIL" << std::endl;
		break;
	case SSM::NoComputedData:
		std::cout << "NCD" << std::endl;
		break;
	}
}

void Display::Print() {

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);

	DWORD consoleSize = csbi.dwSize.X * csbi.dwSize.Y;
	DWORD written;

	FillConsoleOutputCharacter(hConsole, ' ', consoleSize, { 0, 0 }, &written);
	SetConsoleCursorPosition(hConsole, { 0, 0 });

	std::cout << "**GPS DATA**" << std::endl;
	std::cout << "Altitude: ";
	SSMCheck(aircraftstate.gpsAltitude, aircraftstate.gpsAltitudeSSM);
	std::cout << "Ground Speed: ";
	SSMCheck(aircraftstate.groundSpeed, aircraftstate.groundSpeedSSM);
	std::cout << "Latitude: ";
	SSMCheck(aircraftstate.latitude, aircraftstate.latitudeSSM);
	std::cout << "Longitude: ";
	SSMCheck(aircraftstate.longitude, aircraftstate.longitudeSSM);

	std::cout << "-------------------------" << std::endl;

	std::cout << "**ADC DATA**" << std::endl;
	std::cout << "Baro Altitude: ";
	SSMCheck(aircraftstate.baroAltitude, aircraftstate.baroAltitudeSSM);
	std::cout << "Airspeed: ";
	SSMCheck(aircraftstate.airspeed, aircraftstate.airspeedSSM);


	std::cout << "-------------------------" << std::endl;

	std::cout << "**IRS DATA**" << std::endl;
	std::cout << "Heading: ";
	SSMCheck(aircraftstate.heading, aircraftstate.headingSSM);
	std::cout << "Roll: ";
	SSMCheck(aircraftstate.roll, aircraftstate.rollSSM);
	std::cout << "Pitch: ";
	SSMCheck(aircraftstate.pitch, aircraftstate.pitchSSM);

	std::cout << "-------------------------" << std::endl;

	std::cout << "**RADIO DATA**" << std::endl;
	std::cout << "Radio Altitude: ";
	SSMCheck(aircraftstate.radioAltitude, aircraftstate.radioAltitudeSSM);


}

void Display::Run() {
	while (true) {
		ReceiveState();
		Print();
	}
}