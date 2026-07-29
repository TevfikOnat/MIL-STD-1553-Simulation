#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "ARINCWord.h"
#include "States.h"
#include "Display.h"

#pragma comment(lib, "ws2_32.lib")

int main() {
	
	Display display;

	display.Run();

}