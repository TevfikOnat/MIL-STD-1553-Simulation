#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "ARINCWord.h"

#pragma comment(lib, "ws2_32.lib")

int main() {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cout << "WSAStartup failed" << std::endl;
		return -1;
	}
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock == INVALID_SOCKET)
	{
		std::cout << "Invalid Socket" << std::endl;
		return -1;
	}

	sockaddr_in address{};
	address.sin_family = AF_INET;
	address.sin_port = htons(8002);
	address.sin_addr.s_addr = INADDR_ANY;

	sockaddr_in sender{};

	if (bind(sock, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == SOCKET_ERROR) {
		std::cout << "Bind failed: " << WSAGetLastError() << std::endl;
		return -1;
	}

	ARINCWord word;
	uint32_t raw;

	while (true) {
		int senderSize = sizeof(sender);

		int bytesReceived = recvfrom(sock, reinterpret_cast<char*>(&raw), sizeof(raw), 0,
			reinterpret_cast<sockaddr*>(&sender), &senderSize);

		if (bytesReceived == SOCKET_ERROR) {
			std::cout << "Receive failed: " << WSAGetLastError() << std::endl;
		}
		else {
			word = DecodeARINCWord(raw);
			
		}
	}
}