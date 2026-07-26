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

    rt1.sin_family = AF_INET;
    rt1.sin_port = htons(Ports::RemoteTerminal);
    InetPton(AF_INET, "127.0.0.1", &rt1.sin_addr);

    buscontroller.sin_family = AF_INET;
    buscontroller.sin_port = htons(Ports::BusController);
    InetPton(AF_INET, "127.0.0.1", &buscontroller.sin_addr);

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
        ForwardToBC(reinterpret_cast<const char*>(&buffer), bytesReceived);

    }
    else if (ntohs(sender.sin_port) == Ports::RemoteTerminal) {
        ForwardToRT(reinterpret_cast<const char*>(&buffer), bytesReceived);
    }
}

void BUS::ForwardToRT(const char* buffer, int bytesReceived){

    
    sendto(sock, buffer, bytesReceived, 0,
        reinterpret_cast<sockaddr*>(&buscontroller),
        sizeof(buscontroller));
    

}

void BUS::ForwardToBC(const char* buffer, int bytesReceived) {
	
	sendto(sock, buffer, bytesReceived, 0,
		reinterpret_cast<sockaddr*>(&rt1),
			sizeof(rt1));
	
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