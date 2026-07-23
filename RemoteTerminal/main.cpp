#include <winsock2.h>
#include <iostream>
#include <array>
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

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;

    bind(
        sock,
        reinterpret_cast<sockaddr*>(&address),
        sizeof(address)
    );

    sockaddr_in sender{};
    int senderSize = sizeof(sender);

    uint16_t CommandWord;
    uint16_t DataWord;

    int bytesReceived = recvfrom(sock, reinterpret_cast<char*>(&CommandWord), sizeof(CommandWord), 0,
        reinterpret_cast<sockaddr*>(&sender),
        &senderSize);    

    array<uint16_t, 32> memory;
    uint8_t memAddress = 0;

    
    DecodedCommand command = DecodeCMD(CommandWord);

    int rtAddress = command.rtAddress;
    int transmit = command.transmit;
    int subAddress = command.subAddress;
    int wordCount = command.wordCount;

    if (static_cast<int>(command.rtAddress) == 5) {

        cout << "RT Address: " << rtAddress << endl;
        cout << "Transmit/Receive: " << transmit << endl;
        cout << "Sub Address: " << subAddress << endl;
        cout << "Word Count: " << wordCount << endl;

        if (transmit == 0) {
            int DataReceived = recvfrom(sock, reinterpret_cast<char*>(&DataWord), sizeof(DataWord), 0,
                reinterpret_cast<sockaddr*>(&sender),
                &senderSize);
            memory[subAddress] = static_cast<int>(DataWord);
            cout << "Data " << memory[subAddress] << " Written on the address: " << subAddress;
        }

        if (transmit == 1) {

        }
    }

    WSACleanup();
    return 0;
}

