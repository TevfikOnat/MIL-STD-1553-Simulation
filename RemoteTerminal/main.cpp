#include <winsock2.h>
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

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;

    CommandWord cmd;


    bind(
        sock,
        reinterpret_cast<sockaddr*>(&address),
        sizeof(address)
    );

    //char buffer[1024];

    sockaddr_in sender{};
    int senderSize = sizeof(sender);

    int bytesReceived = recvfrom(sock, reinterpret_cast<char*>(&cmd), sizeof(cmd), 0,
        reinterpret_cast<sockaddr*>(&sender),
        &senderSize);
    
    //buffer[bytesReceived] = '\0';

    cout << "Received " << bytesReceived << " bytes\n";

    cout << "RT Address : " << (int)cmd.rtAddress << endl;
    cout << "Transmit   : " << (int)cmd.transmit << endl;
    cout << "SubAddress : " << (int)cmd.subAddress << endl;
    cout << "WordCount  : " << (int)cmd.wordCount << endl;

    WSACleanup();
    return 0;
}

