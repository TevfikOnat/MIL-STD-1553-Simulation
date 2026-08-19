#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "ARINCWord.h"
#include "States.h"
#include "Display.h"

#pragma comment(lib, "ws2_32.lib")

int main() {
	SetConsoleTitleW(L"Display");

	char title[256];

	GetConsoleTitleA(title, sizeof(title));

	std::cout << "Window title: " << title << std::endl;

	Display display;

	display.Run();

}