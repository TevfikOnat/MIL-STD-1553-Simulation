#include "BusController.h"
#include "Ports.h"

using namespace std;


void BusController::InitializeSocket() {

    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "WSAStartup failed" << endl;
        
    }

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (sock == INVALID_SOCKET)
    {
        cout << "Invalid Socket" << endl;
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(Ports::BusController);
    address.sin_addr.s_addr = INADDR_ANY;

    bind(
        sock,
        reinterpret_cast<sockaddr*>(&address),
        sizeof(address)
    );

    sockaddr_in destination;
    destination.sin_family = AF_INET;
    destination.sin_port = htons(Ports::RemoteTerminal);
    InetPton(AF_INET, "127.0.0.1", &destination.sin_addr);
    int destinationSize = sizeof(destination);

}



void BusController::SendCommand() {

}

void BusController::Run() {
    InitializeSocket();
}