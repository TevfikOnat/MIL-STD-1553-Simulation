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

void BusMonitor::ReceiveCommand() {
    int CMDReceived = recvfrom(sock, reinterpret_cast<char*>(&CommandWord), sizeof(CommandWord), 0,
        reinterpret_cast<sockaddr*>(&sender),
        &senderSize);

    if (CMDReceived == SOCKET_ERROR) {
        cout << "Error occured receiving CommandWord" << endl;
    }

    
    commandSent++;
    wordsSent++;

    command = DecodeCMD(CommandWord);
    /*
    cout << "COMMAND " << "RT" << static_cast<int>(command.rtAddress);
    if (command.transmit == 0) {
        cout << " RECEIVE" << endl;
    }
    else {
        cout << " TRANSMIT" << endl;
    }
    cout << "WORD COUNT: " << static_cast<int>(command.wordCount) << endl;*/
}

void BusMonitor::ReceiveData() {
    uint16_t DataWord;
    int DataReceived;

    for (int i = 0;i < command.wordCount;i++) {
        DataReceived = recvfrom(sock, reinterpret_cast<char*>(&DataWord), sizeof(DataWord), 0,
            reinterpret_cast<sockaddr*>(&sender), &senderSize);
        
        if (DataReceived == SOCKET_ERROR) {
            //cout << "DATA RECEIVE FAILED: " << WSAGetLastError() << endl;
        }
        else {
            //cout << "DATA " << i + 1 << ": " << DataWord << endl;
            wordsSent++;
            dataSent++;
        }
    }
}

void BusMonitor::ReceiveStatus() {
    uint16_t StatusWord;
    int StatusReceived = recvfrom(sock, reinterpret_cast<char*>(&StatusWord), sizeof(StatusWord), 0,
        reinterpret_cast<sockaddr*>(&sender), &senderSize);

    if (StatusReceived == SOCKET_ERROR) {
        //cout << "STATUS RECEIVE FAILED " << endl;
    }
    else {
        decodedstat = DecodeStatus(StatusWord);
        statusSent++;
        wordsSent++;/*
            if (decodedstat.messageError == 1 || decodedstat.busy == 1) {
                cout << (decodedstat.messageError == 1 ? "STATUS: MESSAGE ERROR" : "STATUS: RT BUSY") << endl;
            }
            else {
                cout << "STATUS: OK" << endl;
            }*/
    }
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

    std::cout << "BC->RT" << static_cast<int>(command.rtAddress) << " : Command" << endl;

    if (decodedstat.busy == 1) {
        std::cout << "RT" << static_cast<int>(command.rtAddress) << "->BC" << " : BUSY";
        if (decodedstat.messageError == 1) {
            std::cout << " | MESSAGE ERROR " << endl;
        }
        else
            std::cout << std::endl;
    }

    else if (decodedstat.messageError == 1) {
        std::cout << "RT" << static_cast<int>(command.rtAddress) << "->BC" << " : MESSAGE ERROR";
        if (decodedstat.busy == 1) {
            std::cout << " | BUSY " << endl;
        }
        else
            std::cout << std::endl;
    }

    else {
        std::cout << "RT" << static_cast<int>(command.rtAddress) << "->BC" << " : OK" << endl;
    }
}

void BusMonitor::Run() {
    while (true) {
        ReceiveCommand();
        if (command.transmit == 0) {
            ReceiveData();
            ReceiveStatus();
            cout << endl;
        }
        else if (command.transmit == 1) {
            ReceiveStatus();
            ReceiveData();
            cout << endl;
        }
        Print();
    }
}