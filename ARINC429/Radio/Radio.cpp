#include "Radio.h"
#include <iostream>
#include <chrono>
#include <thread>

Radio::Radio() {
	if (!InitializeSocket())
	{
		throw std::runtime_error("Socket initialization failed");
	}
}

Radio::~Radio() {
	CloseSocket();
}

bool Radio::InitializeSocket() {
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
	address.sin_port = htons(8005);
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

void Radio::CloseSocket() {
	closesocket(sock);
	WSACleanup();
}

void Radio::UpdateAltitude() {
	radioState.radioAltitude += altitudeDistribution(generator);
	radioWord.label = Label::RadioAltitude;
	radioWord.data = radioState.radioAltitude;
}

void Radio::SendARINCWord(uint32_t raw) {
	int wordSent = sendto(sock, reinterpret_cast<char*>(&raw), sizeof(raw), 0,
		reinterpret_cast<sockaddr*>(&destination),
		sizeof(destination));
	if (wordSent == SOCKET_ERROR) {
		std::cout << "sendto failed: " << WSAGetLastError() << std::endl;
	}
}

void Radio::Run() {
	while (true) {
		UpdateAltitude();
		SendARINCWord(CreateARINCWord(radioWord));
		Delay(200);
	}
}