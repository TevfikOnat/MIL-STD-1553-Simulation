#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "ARINCWord.h"
#include "GPS.h"

#pragma comment(lib, "ws2_32.lib")

int main() {
    
	GPS gps;

	gps.Run();
	return 0;		

}