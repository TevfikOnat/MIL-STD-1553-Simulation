#include "Display.h"
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
    address.sin_port = htons(8000);
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

void Display::ReceiveARINCWord() {
	int senderSize = sizeof(sender);
	int bytesReceived = recvfrom(sock, reinterpret_cast<char*>(&raw), sizeof(raw), 0,
		reinterpret_cast<sockaddr*>(&sender), &senderSize);
	if (bytesReceived == SOCKET_ERROR) {
		std::cout << "Receive failed: " << WSAGetLastError() << std::endl;
	}
	else {
		word = DecodeARINCWord(raw);
	}
}

void Display::Update() {
	switch (word.label) {
	case Label::GPSAltitude:
		gpsState.altitude = word.data;
		break;
	case Label::groundSpeed:
		gpsState.groundSpeed = word.data;
		break;
	case Label::Latitude:
		gpsState.latitude = word.data;
		break;
	case Label::Longitude:
		gpsState.longitude = word.data;
		break;
	case Label::BaroAltitude:
		adcState.baroAltitude = word.data;
		break;
	case Label::Airspeed:
		adcState.airspeed = word.data;
		break;
	case Label::Heading:
		irsState.heading = word.data;
		break;
	case Label::Roll:
		irsState.roll = word.data;
		break;
	case Label::Pitch:
		irsState.pitch = word.data;
		break;
	case Label::RadioAltitude:
		radioState.radioAltitude = word.data;
		break;
	default:
		std::cout << "Error: Unknown label" << std::endl;
	}
}

void Display::Print() {

	std::system("cls");
	std::cout << "**GPS DATA**" << std::endl;
	std::cout << "Altitude: " << gpsState.altitude << std::endl;
	std::cout << "Ground Speed: " << gpsState.groundSpeed << std::endl;
	std::cout << "Latitude: " << gpsState.latitude << std::endl;
	std::cout << "Longitude: " << gpsState.longitude << std::endl;

	std::cout << "-------------------------" << std::endl;
	std::cout << "**ADC DATA**" << std::endl;
	std::cout << "Baro Altitude: " << adcState.baroAltitude << std::endl;
	std::cout << "Airspeed: " << adcState.airspeed << std::endl;

	std::cout << "-------------------------" << std::endl;
	std::cout << "**IRS DATA**" << std::endl;
	std::cout << "Heading: " << irsState.heading << std::endl;
	std::cout << "Roll: " << irsState.roll << std::endl;
	std::cout << "Pitch: " << irsState.pitch << std::endl;

	std::cout << "-------------------------" << std::endl;
	std::cout << "**RADIO DATA**" << std::endl;
	std::cout << "Radio Altitude: " << radioState.radioAltitude << std::endl;

}

void Display::Run() {
	while (true) {
		for (int i = 0; i < 10; i++)
		{
			ReceiveARINCWord();
			Update();
		}
		Print();
	}
}