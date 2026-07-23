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

    CommandWord cmd;

    cmd.rtAddress = 9;
    cmd.transmit = 1;
    cmd.subAddress = 3;
    cmd.wordCount = 1;

    const char* message = "Live Long Glorious Gilgamesh";

    char buffer[1024];

    int bytesSent = sendto(sock, reinterpret_cast<char*>(&cmd), sizeof(cmd), 0,
        reinterpret_cast<sockaddr*>(&destination),
        sizeof(destination));

    buffer[bytesSent] = '\0';

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

