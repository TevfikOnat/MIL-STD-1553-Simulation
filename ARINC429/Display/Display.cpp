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


void Display::Print() {

	std::system("cls");
	std::cout << "**GPS DATA**" << std::endl;
	std::cout << "Altitude: " << aircraftstate.gpsAltitude << std::endl;
	std::cout << "Ground Speed: " << aircraftstate.groundSpeed << std::endl;
	std::cout << "Latitude: " << aircraftstate.latitude << std::endl;
	std::cout << "Longitude: " << aircraftstate.longitude << std::endl;

	std::cout << "-------------------------" << std::endl;
	std::cout << "**ADC DATA**" << std::endl;
	std::cout << "Baro Altitude: " << aircraftstate.baroAltitude << std::endl;
	std::cout << "Airspeed: " << aircraftstate.airspeed << std::endl;

	std::cout << "-------------------------" << std::endl;
	std::cout << "**IRS DATA**" << std::endl;
	std::cout << "Heading: " << aircraftstate.heading << std::endl;
	std::cout << "Roll: " << aircraftstate.roll << std::endl;
	std::cout << "Pitch: " << aircraftstate.pitch << std::endl;

	std::cout << "-------------------------" << std::endl;
	std::cout << "**RADIO DATA**" << std::endl;
	std::cout << "Radio Altitude: " << aircraftstate.radioAltitude << std::endl;

}

void Display::Run() {
	while (true) {
		ReceiveState();
		Print();
	}
}