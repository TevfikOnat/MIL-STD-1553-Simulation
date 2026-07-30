#include "MissionComputer.h"
#include "Ports.h"
#include <iostream>

MissionComputer::MissionComputer() {
	if (!InitializeSocket())
	{
		throw std::runtime_error("Socket initialization failed");
	}
}

MissionComputer::~MissionComputer() {
	CloseSocket();
}

bool MissionComputer::InitializeSocket() {
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
	address.sin_port = htons(Ports::MissionComputer);
	address.sin_addr.s_addr = INADDR_ANY;

	destination.sin_family = AF_INET;
	destination.sin_port = htons(Ports::Display);
	destination.sin_addr.s_addr = INADDR_ANY;
	InetPton(AF_INET, "127.0.0.1", &destination.sin_addr);


	if (bind(sock, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == SOCKET_ERROR) {
		std::cout << "Bind failed: " << WSAGetLastError() << std::endl;
		return false;
	}
	return true;
}

void MissionComputer::CloseSocket() {
	closesocket(sock);
	WSACleanup();
}

void MissionComputer::ReceiveARINCWord() {
	int senderSize = sizeof(sender);
	int bytesReceived = recvfrom(sock, reinterpret_cast<char*>(&raw), sizeof(raw), 0,
		reinterpret_cast<sockaddr*>(&sender), &senderSize);
	if (bytesReceived == SOCKET_ERROR) {
		std::cout << "Receive failed: " << WSAGetLastError() << std::endl;
	}
}

bool MissionComputer::DecodeWord() {
	int ones = 0;

	for (int i = 0; i < 32; i++)
	{
		if (raw & (1u << i))
			ones++;
	}

	if (ones % 2 == 0) {
		return false;
	}

	word = DecodeARINCWord(raw);

	return true;
}

void MissionComputer::UpdateState() {
	switch (word.label) {
	case Label::GPSAltitude:
		aircraftState.gpsAltitude = word.data;
		break;
	case Label::groundSpeed:
		aircraftState.groundSpeed = word.data;
		break;
	case Label::Latitude:
		aircraftState.latitude = word.data;
		break;
	case Label::Longitude:
		aircraftState.longitude = word.data;
		break;
	case Label::BaroAltitude:
		aircraftState.baroAltitude = word.data;
		break;
	case Label::Airspeed:
		aircraftState.airspeed = word.data;
		break;
	case Label::Heading:
		aircraftState.heading = word.data;
		break;
	case Label::Roll:
		aircraftState.roll = word.data;
		break;
	case Label::Pitch:
		aircraftState.pitch = word.data;
		break;
	case Label::RadioAltitude:
		aircraftState.radioAltitude = word.data;
		break;
	default:
		std::cout << "Error: Unknown label" << std::endl;
	}
}


void MissionComputer::SendToDisplay() {
	int wordSent = sendto(sock, reinterpret_cast<char*>(&aircraftState), sizeof(aircraftState), 0,
		reinterpret_cast<sockaddr*>(&destination),
		sizeof(destination));
	if (wordSent == SOCKET_ERROR) {
		std::cout << "sendto failed: " << WSAGetLastError() << std::endl;
	}
}

void MissionComputer::Run() {
	while (true) {
		ReceiveARINCWord();

		if (!DecodeWord())
			continue;

		UpdateState();
		SendToDisplay();
		Delay(50);
	}
}