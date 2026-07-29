#include "BUS.h"
#include "Ports.h"
#include "CommandWord.h"
using namespace std;


void BUS::InitializeSocket() {

    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "WSAStartup failed" << endl;
    }

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (sock == INVALID_SOCKET)
    {
        cout << "Invalid Socket" << endl;
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(Ports::BUS);
    address.sin_addr.s_addr = INADDR_ANY;


    for (int i = 0;i < Ports::RemoteTerminals.size();i++) {
        rtSockets[i].sin_family = AF_INET;
        rtSockets[i].sin_port = htons(Ports::RemoteTerminals[i]);
        InetPton(AF_INET, "127.0.0.1", &rtSockets[i].sin_addr);
    }

    buscontroller.sin_family = AF_INET;
    buscontroller.sin_port = htons(Ports::BusController);
    InetPton(AF_INET, "127.0.0.1", &buscontroller.sin_addr);

    busmonitor.sin_family = AF_INET;
    busmonitor.sin_port = htons(Ports::BusMonitor);
    InetPton(AF_INET, "127.0.0.1", &busmonitor.sin_addr);

	if (bind(sock, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == SOCKET_ERROR) {
		cout << "Bind failed: " << WSAGetLastError() << endl;
	}
}

void BUS::ReceivePacket() {
    char buffer[256];

    int bytesReceived = recvfrom(sock, buffer, sizeof(buffer), 0,
        reinterpret_cast<sockaddr*>(&sender),
        &senderSize);

    if (ntohs(sender.sin_port) == Ports::BusController) {
        ForwardToRT(reinterpret_cast<const char*>(&buffer), bytesReceived);
        ForwardToBM(reinterpret_cast<const char*>(&buffer), bytesReceived);
    }
    else if (ntohs(sender.sin_port) >= 7000 && ntohs(sender.sin_port) <= 7004) {
        ForwardToBC(reinterpret_cast<const char*>(&buffer), bytesReceived);
        ForwardToBM(reinterpret_cast<const char*>(&buffer), bytesReceived);

    }
}

void BUS::ForwardToBC(const char* buffer, int bytesReceived){        
    sendto(sock, buffer, bytesReceived, 0,
        reinterpret_cast<sockaddr*>(&buscontroller),
        sizeof(buscontroller)); 
}

void BUS::ForwardToRT(const char* buffer, int bytesReceived) {
    for (int i = 0; i < Ports::RemoteTerminals.size(); i++) {
        sendto(sock, buffer, bytesReceived, 0,
            reinterpret_cast<sockaddr*>(&rtSockets[i]),
            sizeof(rtSockets[i]));
    }
}

void BUS::ForwardToBM(const char* buffer, int bytesReceived) {
    sendto(sock, buffer, bytesReceived, 0,
        reinterpret_cast<sockaddr*>(&busmonitor),
        sizeof(busmonitor));
}

void BUS::Run() {
	while (true) {
		ReceivePacket();
	}
}

BUS::BUS() {
	InitializeSocket();
}

BUS::~BUS()
{
    if (sock != INVALID_SOCKET)
    {
        closesocket(sock);
    }

    WSACleanup();
}