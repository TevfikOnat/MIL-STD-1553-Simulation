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
    int destinationSize = sizeof(destination);
    while (1) {
        uint16_t cmd = EncodeCommandWord();
        DecodedCommand decodedcmd = DecodeCMD(cmd);

        int bytesSent = sendto(sock, reinterpret_cast<char*>(&cmd), sizeof(cmd), 0,
            reinterpret_cast<sockaddr*>(&destination),
            sizeof(destination));

        if (bytesSent == SOCKET_ERROR)
        {
            cout << "Send failed: " << WSAGetLastError() << endl;
        }
        else
        {
            cout << "Sent " << bytesSent << " bytes to the rt addres " << static_cast<int>(DecodeCMD(cmd).rtAddress) << endl;
        }

        if (decodedcmd.transmit == 0) {
            uint16_t data = 0;
            cout << "Data to send: ";
            cin >> data;
            int DataSent = sendto(sock, reinterpret_cast<char*>(&data), sizeof(data), 0,
                reinterpret_cast<sockaddr*>(&destination),
                sizeof(destination));

        }

        if (decodedcmd.transmit == 1) {
            uint16_t DataWord;
            int DataReceived = recvfrom(sock, reinterpret_cast<char*>(&DataWord), sizeof(DataWord), 0,
                reinterpret_cast<sockaddr*>(&destination), &destinationSize);
            cout << "Received Data: " << DataWord << endl;

        }
    }
    closesocket(sock);
    WSACleanup();

    cin.get();
    return 0;
}

