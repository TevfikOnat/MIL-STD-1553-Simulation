#pragma once

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

class BusController {
public:
	void Run();
private:
	void InitializeSocket();

	void SendCommand();
	void SendData();

	void ReceiveData();
	void ReceiveStatus();
};