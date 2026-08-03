#include "BusMonitor.h"

using namespace std;

BusMonitor::BusMonitor() {
    InitializeSocket();
}

BusMonitor::~BusMonitor() {
    CloseSocket(sock);
}

void BusMonitor::InitializeSocket() {
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "WSAStartup failed" << endl;
    }

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (sock == INVALID_SOCKET)
    {
        cout << "Invalid Socket" << endl;
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(Ports::BusMonitor);
    address.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == SOCKET_ERROR) {
        cout << "Bind failed: " << WSAGetLastError() << endl;
    }
}

void BusMonitor::CloseSocket(SOCKET sock) {
    closesocket(sock);
    WSACleanup();
}

void BusMonitor::ReceivePacket() {

    int packetReceived = recvfrom(sock, reinterpret_cast<char*>(&packet), sizeof(packet), 0,
        reinterpret_cast<sockaddr*>(&sender),
        &senderSize);

    switch (packet.sync) {
    case Sync::COMMAND:
        ReceiveCommand(packet.Word);
        break;
    case Sync::DATA:
        ReceiveData(packet.Word);
        break;
    case Sync::STATUS:
        ReceiveStatus(packet.Word);
        ErrorLog();
        break;
    }

}

void BusMonitor::ReceiveCommand(uint16_t CommandWord) {
    commandSent++;
    wordsSent++;

    command = DecodeCMD(CommandWord);
}

void BusMonitor::ReceiveData(uint16_t DataWord) {
    wordsSent++;
    dataSent++;
}

void BusMonitor::ReceiveStatus(uint16_t StatusWord) {
    decodedstat = DecodeStatus(StatusWord);
    statusSent++;
    wordsSent++;
}

void BusMonitor::ClearScreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    DWORD consoleSize = csbi.dwSize.X * csbi.dwSize.Y;
    DWORD written;

    FillConsoleOutputCharacter(hConsole, ' ', consoleSize, { 0, 0 }, &written);
    SetConsoleCursorPosition(hConsole, { 0, 0 });
}

void BusMonitor::Print() {

    ClearScreen();
    std::cout << "----------BUS MONITOR----------" << std::endl;
    std::cout << "Words Sent: " << wordsSent << std::endl;
    std::cout << "Command Words: " << commandSent << std::endl;
    std::cout << "Data Words: " << dataSent << std::endl;
    std::cout << "Status Words: " << statusSent << std::endl;

    std::cout << std::fixed << std::setprecision(1);
    for (int i = 0;i < 4;i++) {
        std::cout << "RT" << i + 1 << "->"
            << " Msg: " << static_cast<int>(RTs[i].messageCount)
            << "  Busy: " << static_cast<int>(RTs[i].busyCount) << " (" << RTs[i].busyPercent << "%)"
            << "  Error: " << static_cast<int>(RTs[i].meCount) << " (" << RTs[i].mePercent << "%)"
            << '\n';
    }
}

void BusMonitor::ErrorLog() {
    Terminal = static_cast<int>(command.rtAddress);
    TerminalAddress = Terminal - 1;

    if (TerminalAddress < 0 || TerminalAddress >= 4) {
        return;
    }

    if (decodedstat.busy == 1) {
        RTs[TerminalAddress].busyCount++;
    }

    if (decodedstat.messageError == 1) {
        RTs[TerminalAddress].meCount++;
    }
    RTs[TerminalAddress].messageCount++;
    
    RTs[TerminalAddress].busyPercent = (RTs[TerminalAddress].busyCount / RTs[TerminalAddress].messageCount) * 100.0f;
    RTs[TerminalAddress].mePercent = (RTs[TerminalAddress].meCount / RTs[TerminalAddress].messageCount) * 100.0f;
}

void BusMonitor::Run() {
	lastPrintTime = std::chrono::steady_clock::now();
	while (true) {
		ReceivePacket();
		auto now = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastPrintTime).count();
		if (elapsed >= 50) {
			Print();
			lastPrintTime = now;
		}
	}
}