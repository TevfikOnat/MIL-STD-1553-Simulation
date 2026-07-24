#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "Ports.h"
#include "CommandWord.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    WSADATA wsaData;

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
    address.sin_port = htons(Ports::BUS);
    address.sin_addr.s_addr = INADDR_ANY;

    sockaddr_in rt1{};
    rt1.sin_family = AF_INET;
    rt1.sin_port = htons(Ports::RemoteTerminal);
    InetPton(AF_INET, "127.0.0.1", &rt1.sin_addr);

    bind(
        sock,
        reinterpret_cast<sockaddr*>(&address),
        sizeof(address)
    );

    uint16_t CommandWord;

    sockaddr_in sender{};
    int senderSize = sizeof(sender);

    int bytesReceived = recvfrom(sock, reinterpret_cast<char*>(&CommandWord), sizeof(CommandWord), 0,
        reinterpret_cast<sockaddr*>(&sender),
        &senderSize);

    DecodedCommand command = DecodeCMD(CommandWord);

    int rtAddress = command.rtAddress;
    int transmit = command.transmit;
    int subAddress = command.subAddress;
    int wordCount = command.wordCount;

    cout << ntohs(sender.sin_port) << endl;
    cout << "RT address: " << rtAddress;

    int bytesSent = sendto(sock, reinterpret_cast<char*>(&CommandWord), sizeof(CommandWord), 0,
        reinterpret_cast<sockaddr*>(&rt1),
        sizeof(rt1));

    cin.get();

}