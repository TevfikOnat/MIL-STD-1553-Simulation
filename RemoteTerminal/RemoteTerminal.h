#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <array>
#include "CommandWord.h"
#include "StatusWord.h"
#include "Ports.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

class RemoteTerminal {
public:
	RemoteTerminal();
	~RemoteTerminal();

	void Run();
private:
	void InitializeSocket();
	void CloseSocket(SOCKET sock);

	void ReceiveCommand();

	void ReceiveData(int wordCount, int subAddress);
	void SendData(int WordCount, int subAddress);
	void SendStatus(int rtAddress);

private:
	SOCKET sock = INVALID_SOCKET;
	sockaddr_in address{};
	sockaddr_in destination{};
	sockaddr_in sender{};
	int destinationSize = sizeof(destination);
	int senderSize = sizeof(sender);

	DecodedCommand command;

	array<uint16_t, 32> memory;
	uint8_t memAddress = 0;

	uint16_t CommandWord;
	uint16_t DataWord;
	uint16_t statusWord;
	
	bool messageerror = FALSE;
	bool busy = FALSE;
};