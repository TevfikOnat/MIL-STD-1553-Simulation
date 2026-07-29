#include "IRS.h"
#include <iostream>

IRS::IRS() {
	if (!InitializeSocket()) {
		std::cerr << "Failed to initialize socket." << std::endl;
	}
}

IRS::~IRS() {
	CloseSocket();
}

bool IRS::InitializeSocket() {
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
	address.sin_port = htons(8004);
	address.sin_addr.s_addr = INADDR_ANY;

	destination.sin_family = AF_INET;
	destination.sin_port = htons(8000);
	destination.sin_addr.s_addr = INADDR_ANY;
	InetPton(AF_INET, "127.0.0.1", &destination.sin_addr);


	if (bind(sock, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == SOCKET_ERROR) {
		std::cout << "Bind failed: " << WSAGetLastError() << std::endl;
		return false;
	}
	return true;
}

void IRS::CloseSocket() {
	closesocket(sock);
	WSACleanup();
}

void IRS::UpdateHeading() {
	irsState.heading += headingDistribution(generator);
	irsWord.label = Label::Heading;
	irsWord.data = irsState.heading;
}

void IRS::UpdateRoll() {
	irsState.roll += rollDistribution(generator);
	irsWord.label = Label::Roll;
	irsWord.data = irsState.roll;
}

void IRS::UpdatePitch() {
	irsState.pitch += pitchDistribution(generator);
	irsWord.label = Label::Pitch;
	irsWord.data = irsState.pitch;
}


void IRS::SendARINCWord(uint32_t raw) {
	int wordSent = sendto(sock, reinterpret_cast<char*>(&raw), sizeof(raw), 0,
		reinterpret_cast<sockaddr*>(&destination),
		sizeof(destination));
	if (wordSent == SOCKET_ERROR) {
		std::cout << "sendto failed: " << WSAGetLastError() << std::endl;
	}
}

void IRS::Run() {
	while (true) {
		UpdateHeading();
		SendARINCWord(CreateARINCWord(irsWord));
		Delay(200);
		UpdateRoll();
		SendARINCWord(CreateARINCWord(irsWord));
		Delay(200);
		UpdatePitch();
		SendARINCWord(CreateARINCWord(irsWord));
		Delay(200);
	}
}
