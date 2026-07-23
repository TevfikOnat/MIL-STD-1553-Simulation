#include <winsock2.h>
#include <iostream>

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

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;


    uint16_t CommandWord;

    uint8_t rtAddress;
    uint8_t transmit;
    uint8_t subAddress;
    uint8_t wordCount;

    bind(
        sock,
        reinterpret_cast<sockaddr*>(&address),
        sizeof(address)
    );


    sockaddr_in sender{};
    int senderSize = sizeof(sender);

    int bytesReceived = recvfrom(sock, reinterpret_cast<char*>(&CommandWord), sizeof(CommandWord), 0,
        reinterpret_cast<sockaddr*>(&sender),
        &senderSize);
    

    rtAddress = (CommandWord >> 11) & (0x1F);
    transmit = (CommandWord >> 10) & (0x01);
    subAddress = (CommandWord >> 5) & (0x1F);
    wordCount = CommandWord & (0x1F);


    cout << "RT Address: " << (int)rtAddress << endl;
    cout << "Transmit/Receive: " << (int)transmit << endl;
    cout << "Sub Address: " << (int)subAddress << endl;
    cout << "Word Count: " << (int)wordCount << endl;

    WSACleanup();
    return 0;
}

