#include "ADC.h"
#include <iostream>

ADC::ADC() {
	if (!InitializeSocket())
	{
		throw std::runtime_error("Socket initialization failed");
	}
}

ADC::~ADC() {
	CloseSocket();
}

bool ADC::InitializeSocket() {
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
	address.sin_port = htons(8003);
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

void ADC::CloseSocket() {
	closesocket(sock);
	WSACleanup();
}

void ADC::UpdateAirspeed() {
	adcState.airspeed += airspeedDistribution(generator);
	adcWord.label = Label::Airspeed;
	adcWord.data = adcState.airspeed;
}

void ADC::UpdateBaroAltitude() {
	adcState.baroAltitude += baroaltitudeDistribution(generator);
	adcWord.label = Label::BaroAltitude;
	adcWord.data = adcState.baroAltitude;
}

void ADC::SendARINCWord(uint32_t raw) {
	int sendResult = sendto(sock, reinterpret_cast<const char*>(&raw), sizeof(raw), 0,
		reinterpret_cast<sockaddr*>(&destination), sizeof(destination));
	if (sendResult == SOCKET_ERROR) {
		std::cout << "Send failed: " << WSAGetLastError() << std::endl;
	}
}

void ADC::Run() {
	while (true) {
		UpdateAirspeed();
		SendARINCWord(CreateARINCWord(adcWord));
		Delay(200);
		UpdateBaroAltitude();
		SendARINCWord(CreateARINCWord(adcWord));
		Delay(200);
	}
}