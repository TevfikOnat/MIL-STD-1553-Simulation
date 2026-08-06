#include "MissionComputer.h"
#include "Ports.h"
#include "CommandWord.h"
#include "StatusWord.h"
#include <iostream>
#include <timeapi.h>
#pragma comment(lib, "winmm.lib")

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

	sockARINC = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sock1553 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (sockARINC == INVALID_SOCKET)
	{
		std::cout << "429 Invalid Socket" << std::endl;
		return false;
	}
	if (sock1553 == INVALID_SOCKET)
	{
		std::cout << "1553 Invalid Socket" << std::endl;
		return false;
	}

	ARINCaddress.sin_family = AF_INET;
	ARINCaddress.sin_port = htons(Ports::MissionComputer);
	ARINCaddress.sin_addr.s_addr = INADDR_ANY;

	BUSaddress.sin_family = AF_INET;
	BUSaddress.sin_port = htons(Ports::BusController);
	BUSaddress.sin_addr.s_addr = INADDR_ANY;

	displayDestination.sin_family = AF_INET;
	displayDestination.sin_port = htons(Ports::Display);
	displayDestination.sin_addr.s_addr = INADDR_ANY;
	InetPton(AF_INET, "127.0.0.1", &displayDestination.sin_addr);

	busDestination.sin_family = AF_INET;
	busDestination.sin_port = htons(Ports::BUS);
	busDestination.sin_addr.s_addr = INADDR_ANY;
	InetPton(AF_INET, "127.0.0.1", &busDestination.sin_addr);


	if (bind(sockARINC, reinterpret_cast<sockaddr*>(&ARINCaddress), sizeof(ARINCaddress)) == SOCKET_ERROR) {
		std::cout << "ARINC Bind failed: " << WSAGetLastError() << std::endl;
		return false;
	}
	if (bind(sock1553, reinterpret_cast<sockaddr*>(&BUSaddress), sizeof(BUSaddress)) == SOCKET_ERROR) {
		std::cout << "BUS Bind failed: " << WSAGetLastError() << std::endl;
		return false;
	}
	return true;
}

void MissionComputer::CloseSocket() {
	closesocket(sockARINC);
	closesocket(sock1553);
	WSACleanup();
}

void MissionComputer::ReceiveARINCWord() {
	int senderSize = sizeof(sender);
	int bytesReceived = recvfrom(sockARINC, reinterpret_cast<char*>(&raw), sizeof(raw), 0,
		reinterpret_cast<sockaddr*>(&sender), &senderSize);
	if (bytesReceived == SOCKET_ERROR) {
		std::cout << "Receive failed: " << WSAGetLastError() << std::endl;
	}
}

void MissionComputer::Bus1553Scheduler() {
	using namespace std::chrono;

	// Sistem saatini referans alıyoruz
	auto now = steady_clock::now();
	lastRT1Time = now;
	lastRT2Time = now;
	lastRT3Time = now;
	lastRT4Time = now;

	while (isRunning) {
		// Şu anki hassas zamanı al
		now = steady_clock::now();


		if (duration_cast<milliseconds>(now - lastRT1Time).count() >= 50) {

			{
				// Thread 1 tam bu esnada aircraftState'e yazma yapıyorsa bekle.
				std::lock_guard<std::mutex> lock(stateMutex);
				SendToRT1();
			} // Kilit hemen açılır

			lastRT1Time = now; // RT1 için zaman damgasını güncelle
		}

		if (duration_cast<milliseconds>(now - lastRT2Time).count() >= 100) {
			{
				std::lock_guard<std::mutex> lock(stateMutex);
				SendToRT2();
			}
			lastRT2Time = now;
		}

		if (duration_cast<milliseconds>(now - lastRT3Time).count() >= 200) {
			{
				std::lock_guard<std::mutex> lock(stateMutex);
				SendToRT3();
			}
			lastRT3Time = now;

			if (duration_cast<milliseconds>(now - lastRT4Time).count() >= 1000) {
				{
					std::lock_guard<std::mutex> lock(stateMutex);
					SendToRT4();
				}
				lastRT4Time = now;
			}

			// CPU'nun tek çekirdeğini %100 yükte çalıştırmamak için 1 ms'lik mikro uyku
			std::this_thread::sleep_for(milliseconds(1));
		}
	}
}

void MissionComputer::ARINCReceiverLoop() {
	while (isRunning) {
		ReceiveARINCWord();

		if (DecodeWord()) {
			std::lock_guard<std::mutex> lock(stateMutex);

			UpdateState(); 
			SendToDisplay();
		}
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
	if (word.ssm == SSM::NormalOperation) {
		switch (word.label)
		{
		case Label::GPSAltitude:
			aircraftState.gpsAltitudeSSM = word.ssm;
			aircraftState.gpsAltitude = word.data;
			break;
		case Label::groundSpeed:
			aircraftState.groundSpeedSSM = word.ssm;
			aircraftState.groundSpeed = word.data;
			break;
		case Label::Latitude:
			aircraftState.latitudeSSM = word.ssm;
			aircraftState.latitude = word.data;
			break;
		case Label::Longitude:
			aircraftState.longitudeSSM = word.ssm;
			aircraftState.longitude = word.data;
			break;
		case Label::BaroAltitude:
			aircraftState.baroAltitudeSSM = word.ssm;
			aircraftState.baroAltitude = word.data;
			break;
		case Label::Airspeed:
			aircraftState.airspeedSSM = word.ssm;
			aircraftState.airspeed = word.data;
			break;
		case Label::Heading:
			aircraftState.headingSSM = word.ssm;
			aircraftState.heading = word.data;
			break;
		case Label::Roll:
			aircraftState.rollSSM = word.ssm;
			aircraftState.roll = word.data;
			break;
		case Label::Pitch:
			aircraftState.pitchSSM = word.ssm;
			aircraftState.pitch = word.data;
			break;
		case Label::RadioAltitude:
			aircraftState.radioAltitudeSSM = word.ssm;
			aircraftState.radioAltitude = word.data;
			break;
		default:
			std::cout << "Error: Unknown label" << std::endl;
		}
	}
}


void MissionComputer::SendToDisplay() {
	int wordSent = sendto(sockARINC, reinterpret_cast<char*>(&aircraftState), sizeof(aircraftState), 0,
		reinterpret_cast<sockaddr*>(&displayDestination),
		sizeof(displayDestination));
	if (wordSent == SOCKET_ERROR) {
		std::cout << "sendto failed: " << WSAGetLastError() << std::endl;
	}
}


void MissionComputer::Send1553Message(uint8_t rtAddress,
									uint8_t subAddress,
									const int16_t* dataWords,
									uint8_t wordCount)
{

	subAddress = (addressroll(generator) < 15 ? 30 : subAddress);
	
	uint16_t cmd = CreateCommandWord(rtAddress, 0, subAddress, wordCount);
	cmdPacket.sync = Sync::COMMAND;
	cmdPacket.Word = cmd;

	sendto(sock1553, reinterpret_cast<char*>(&cmdPacket), sizeof(cmdPacket), 0,
		reinterpret_cast<sockaddr*>(&busDestination),
		sizeof(busDestination));

	for (int i = 0; i < wordCount; i++)
	{
		dataPacket.sync = Sync::DATA;
		dataPacket.Word = dataWords[i];
		sendto(sock1553, reinterpret_cast<const char*>(&dataPacket), sizeof(dataPacket), 0,
			reinterpret_cast<sockaddr*>(&busDestination),
			sizeof(busDestination));
	}
}

void MissionComputer::SendToRT1() {
	int16_t dataWords[]{
	aircraftState.heading,
	aircraftState.roll,
	aircraftState.pitch,
	aircraftState.baroAltitude,
	aircraftState.radioAltitude,
	};
	Send1553Message(1, 0, dataWords, 5);
}

void MissionComputer::SendToRT2() {
	int16_t dataWords[]{
	aircraftState.latitude,
	aircraftState.longitude,
	aircraftState.groundSpeed,
	aircraftState.heading
	};
	Send1553Message(2, 0, dataWords, 4);
}

void MissionComputer::SendToRT3() {
	int16_t dataWords[]{
	aircraftState.heading,
	aircraftState.roll,
	aircraftState.pitch,
	aircraftState.airspeed,
	aircraftState.baroAltitude,
	aircraftState.radioAltitude
	};
	Send1553Message(3, 0, dataWords, 6);
}


void MissionComputer::SendToRT4() {
	int16_t dataWords[]{
	aircraftState.latitude,
	aircraftState.longitude,
	aircraftState.groundSpeed,
	aircraftState.heading,
	};
	Send1553Message(4, 0, dataWords, 4);
}


DecodedStatus MissionComputer::ReceiveStatus() {
	int busDestinationSize = sizeof(busDestination);
	uint16_t StatusWord;
	int StatusReceived = recvfrom(sock1553, reinterpret_cast<char*>(&StatusWord), sizeof(StatusWord), 0,
		reinterpret_cast<sockaddr*>(&busDestination), &busDestinationSize);

	if (StatusReceived == SOCKET_ERROR) {
		cout << "Receive failed: " << WSAGetLastError() << endl;
	}

	DecodedStatus decodedstat = DecodeStatus(StatusWord);

	if (decodedstat.messageError == 1 || decodedstat.busy == 1) {
		cout << "RT reported an error" << endl;
	}

	return decodedstat; //void olsa da olurmuş
}

void MissionComputer::Run() {
	isRunning = true;
	timeBeginPeriod(1);
	std::thread arincThread(&MissionComputer::ARINCReceiverLoop, this);

	Bus1553Scheduler();

	if (arincThread.joinable()) {
		arincThread.join();
	}
}