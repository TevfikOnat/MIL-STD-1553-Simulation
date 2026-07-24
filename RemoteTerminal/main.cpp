#include <winsock2.h>
#include <iostream>
#include <array>
#include "CommandWord.h"
#include "StatusWord.h"

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

      

    array<uint16_t, 32> memory;
    uint8_t memAddress = 0;

    memory[8] = 123;

    uint16_t statusWord;
    bool messageerror = FALSE;
    bool busy = TRUE;

    while (1) {

        int bytesReceived = recvfrom(sock, reinterpret_cast<char*>(&CommandWord), sizeof(CommandWord), 0,
            reinterpret_cast<sockaddr*>(&sender),
            &senderSize);

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
                for (int i = 0;i < wordCount;i++) {
                    int DataReceived = recvfrom(sock, reinterpret_cast<char*>(&DataWord), sizeof(DataWord), 0,
                        reinterpret_cast<sockaddr*>(&sender),
                        &senderSize);
                    memory[subAddress+i] = static_cast<int>(DataWord);
                    cout << "Data " << memory[subAddress + i] << " Written on the address: " << subAddress + i << endl;
                }                
                statusWord = CreateStatusWord(rtAddress, messageerror, busy);
                int StatusSent = sendto(sock, reinterpret_cast<char*>(&statusWord), sizeof(statusWord), 0,
                    reinterpret_cast<sockaddr*>(&sender),
                    sizeof(sender));
            }

            if (transmit == 1) {
                uint16_t data;
                statusWord = CreateStatusWord(rtAddress, messageerror, busy);
                int StatusSent = sendto(sock, reinterpret_cast<char*>(&statusWord), sizeof(statusWord), 0,
                    reinterpret_cast<sockaddr*>(&sender),
                    sizeof(sender));
                DecodedStatus decodedstat = DecodeStatus(statusWord);
                if(decodedstat.messageError == 1){
                    cout << "Error, can't send data." << endl;
                }
                else if (decodedstat.messageError == 1) {
                    cout << "Busy, can't send data." << endl;
                }
                else {
                    for (int i = 0;i < wordCount;i++) {
                        data = memory[subAddress + i];

                        int bytesSent = sendto(sock, reinterpret_cast<char*>(&data), sizeof(data), 0,
                            reinterpret_cast<sockaddr*>(&sender),
                            sizeof(sender));

                        if (bytesSent == SOCKET_ERROR)
                        {
                            cout << "Send failed: " << WSAGetLastError() << endl;
                        }
                        else
                        {
                            cout << "Sent " << bytesSent << " bytes to the BC ";
                        }
                    }
                }
            }
        }
    }
    WSACleanup();
    return 0;
}

