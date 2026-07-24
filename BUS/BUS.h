#pragma once

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

class BUS {
public:
	void Run();

private:
	void InitializeSocket();

	void ReceiveData();


};