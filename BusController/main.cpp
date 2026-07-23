#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include "CommandWord.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;


int main() {
    WSADATA wsaData;
    int port = 8888;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "WSAStartup failed" << endl;
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (sock == INVALID_SOCKET)
    {
        cout << "Invalid Socket" << endl;
        return 1;
    }

    sockaddr_in destination;
    destination.sin_family = AF_INET;
    destination.sin_port = htons(port);
    InetPton(AF_INET, "127.0.0.1", &destination.sin_addr);

    
    uint16_t cmd = CreateCommandWord(5, 0, 9, 7);

    int bytesSent = sendto(sock, reinterpret_cast<char*>(&cmd), sizeof(cmd), 0,
        reinterpret_cast<sockaddr*>(&destination),
        sizeof(destination));

    if (bytesSent == SOCKET_ERROR)
    {
        cout << "Send failed: " << WSAGetLastError() << endl;
    }
    else
    {
        cout << "Sent " << bytesSent << " bytes to the addres " << static_cast<int>(DecodeCMD(cmd).rtAddress);
    }

    if (static_cast<int>(DecodeCMD(cmd).transmit) == 0) {
        uint16_t data = 352;
        int DataSent = sendto(sock, reinterpret_cast<char*>(&data), sizeof(data), 0,
            reinterpret_cast<sockaddr*>(&destination),
            sizeof(destination));

    }
    

    closesocket(sock);
    WSACleanup();

    cin.get();
    return 0;
}

