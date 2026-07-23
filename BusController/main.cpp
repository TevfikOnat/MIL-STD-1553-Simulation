#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

struct CommandWord
{
    uint8_t rtAddress;
    uint8_t transmit;
    uint8_t subAddress;
    uint8_t wordCount;
};

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
        cout << "Socket oluşturulamadı.\n";
        return 1;
    }

    sockaddr_in destination;
    destination.sin_family = AF_INET;
    destination.sin_port = htons(port);
    InetPton(AF_INET, "127.0.0.1", &destination.sin_addr);

    
    uint8_t rtAddress = 2;
    uint8_t transmit = 0;
    uint8_t subAddress = 12;
    uint8_t wordCount = 5;

    uint16_t cmd = 0;
    cmd |= (rtAddress << 11);
    cmd |= (transmit << 10);
    cmd |= (subAddress << 5);
    cmd |= wordCount;

    int bytesSent = sendto(sock, reinterpret_cast<char*>(&cmd), sizeof(cmd), 0,
        reinterpret_cast<sockaddr*>(&destination),
        sizeof(destination));

    if (bytesSent == SOCKET_ERROR)
    {
        cout << "Send failed: " << WSAGetLastError() << endl;
    }
    else
    {
        cout << "Sent " << bytesSent << " bytes." << endl;
    }

    closesocket(sock);
    WSACleanup();

    cin.get();
    return 0;
}

