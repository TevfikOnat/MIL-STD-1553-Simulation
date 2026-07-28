#include "RemoteTerminal.h"

RemoteTerminal::RemoteTerminal() {
    if (!InitializeSocket()) {
        exit(EXIT_FAILURE);
    }
}	

RemoteTerminal::~RemoteTerminal() {
	CloseSocket(sock);
}

bool RemoteTerminal::InitializeSocket() {

    

    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "WSAStartup failed" << endl;
    }

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (sock == INVALID_SOCKET){
        cout << "Invalid Socket" << endl;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    for (port = Ports::RemoteTerminals[0]; port <= Ports::RemoteTerminals.back(); port++) {
        address.sin_port = htons(port);

        if (bind(sock, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == 0) {
            RT = port - 6999;
            break;
        }
        else {

            if (port == Ports::RemoteTerminals.back()) {
                cout << "All ports are full" << endl;
                return false;
            }

            else {
                cout << "Port " << port << " is full" << endl;
            }

        }
    }

    
    cout << "Assigned to the port: " << port << " RT address: " << RT << endl;

    destination.sin_family = AF_INET;
    destination.sin_port = htons(Ports::BUS);
    destination.sin_addr.s_addr = INADDR_ANY;
    InetPton(AF_INET, "127.0.0.1", &destination.sin_addr);

    return true;
}

void RemoteTerminal::CloseSocket(SOCKET sock) {
	closesocket(sock);
	WSACleanup();
}

void RemoteTerminal::ReceiveCommand() {
    int CMDReceived = recvfrom(sock, reinterpret_cast<char*>(&CommandWord), sizeof(CommandWord), 0,
        reinterpret_cast<sockaddr*>(&sender),
        &senderSize);

    if (CMDReceived == SOCKET_ERROR) {
        cout << "Error occured receiving CommandWord" << endl;
    }

    command = DecodeCMD(CommandWord);
}

void RemoteTerminal::ReceiveData(int wordCount,int subAddress) {
    for (int i = 0;i < wordCount;i++) {

        if (subAddress + i == memory.size()) {
            messageerror = true;
            return;
        }

        int DataReceived = recvfrom(sock, reinterpret_cast<char*>(&DataWord), sizeof(DataWord), 0,
            reinterpret_cast<sockaddr*>(&sender),
            &senderSize);
                    

        if (DataReceived == SOCKET_ERROR){
            cout << "Receive failed: " << WSAGetLastError() << endl;
            return;
        }
        else {
            memory[subAddress + i] = DataWord;
            cout << "Data " << memory[subAddress + i] << " Written on the address: " << subAddress + i << endl;
        }
    }
}

void RemoteTerminal::SendData(int wordCount, int subAddress) {
    uint16_t data;
    for (int i = 0;i < wordCount;i++) {

        if (subAddress + i == memory.size()) {
            messageerror = true;
            return;
        }
        else {
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
                cout << "Sent " << bytesSent << " bytes to the BC " << endl;
            }
        }
    }
}

void RemoteTerminal::SendStatus(int rtAddress) {
    statusWord = CreateStatusWord(rtAddress, messageerror, busy);
    int StatusSent = sendto(sock, reinterpret_cast<char*>(&statusWord), sizeof(statusWord), 0,
        reinterpret_cast<sockaddr*>(&sender),
        sizeof(sender));
    DecodedStatus decodedstat = DecodeStatus(statusWord);
    if (decodedstat.messageError == 1) {
        cout << "Error, can't send data." << endl;
    }
    else if (decodedstat.busy == 1) {
        cout << "Busy, can't send data." << endl;
    }
    else {
        cout << "Sent status." << endl;
    }
}

void RemoteTerminal::Run() {
    while (true) {
        ReceiveCommand();
        if (static_cast<int>(command.rtAddress) == RT) {
            if (command.transmit == 0) {
                ReceiveData(command.wordCount, command.subAddress);
                SendStatus(command.rtAddress);
            }
            else if (command.transmit == 1) {
                SendStatus(command.rtAddress);
                SendData(command.wordCount, command.subAddress);
            }
        }
    }
}