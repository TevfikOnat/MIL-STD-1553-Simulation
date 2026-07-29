#include "GPS.h"
#include <iostream>



GPS::GPS() {
	if (!InitializeSocket())
	{
		throw std::runtime_error("Socket initialization failed");
	}
}

GPS::~GPS() {
	CloseSocket();
}

bool GPS::InitializeSocket() {
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
	address.sin_port = htons(8001);
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

void GPS::CloseSocket() {
	closesocket(sock);
	WSACleanup();
}

void GPS::UpdateGPSAltitude() {
	gpsState.altitude += altitudeDistribution(generator);
	gpsWord.label = Label::GPSAltitude;
	gpsWord.data = gpsState.altitude;
}

void GPS::UpdateGroundSpeed() {
	gpsState.groundSpeed += groundSpeedDistribution(generator);
	gpsWord.label = Label::groundSpeed;
	gpsWord.data = gpsState.groundSpeed;
}

void GPS::UpdateLatitude() {
	gpsState.latitude += latitudeDistribution(generator);
	gpsWord.label = Label::Latitude;
	gpsWord.data = gpsState.latitude;
}

void GPS::UpdateLongitude() {
	gpsState.longitude += longitudeDistribution(generator);
	gpsWord.label = Label::Longitude;
	gpsWord.data = gpsState.longitude;
}	

void GPS::SendARINCWord(uint32_t raw) {
	int wordSent = sendto(sock, reinterpret_cast<char*>(&raw), sizeof(raw), 0,
		reinterpret_cast<sockaddr*>(&destination),
		sizeof(destination));
	if (wordSent == SOCKET_ERROR) {
		std::cout << "sendto failed: " << WSAGetLastError() << std::endl;
	}
}

void GPS::Run() {

	while (true) {
		UpdateGPSAltitude();
		SendARINCWord(CreateARINCWord(gpsWord));
		Delay(200);
		UpdateGroundSpeed();
		SendARINCWord(CreateARINCWord(gpsWord));
		Delay(200);
		UpdateLatitude();
		SendARINCWord(CreateARINCWord(gpsWord));
		Delay(200);
		UpdateLongitude();
		SendARINCWord(CreateARINCWord(gpsWord));
		Delay(200);
	}

}