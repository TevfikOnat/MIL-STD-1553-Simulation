#include "RemoteTerminal.h"

RemoteTerminal::RemoteTerminal() {
	InitializeSocket();
}	

RemoteTerminal::~RemoteTerminal() {
	CloseSocket(sock);
}

void RemoteTerminal::InitializeSocket() {
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "WSAStartup failed" << endl;
    }

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (sock == INVALID_SOCKET){
        cout << "Invalid Socket" << endl;
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(Ports::RemoteTerminal);
    address.sin_addr.s_addr = INADDR_ANY;

    destination.sin_family = AF_INET;
    destination.sin_port = htons(Ports::BUS);
    destination.sin_addr.s_addr = INADDR_ANY;
    InetPton(AF_INET, "127.0.0.1", &destination.sin_addr);

    if (bind(sock, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == SOCKET_ERROR) {
        cout << "Bind failed: " << WSAGetLastError() << endl;
    }
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

    cout << "RT Address: " << static_cast<int>(command.rtAddress) << endl;
    cout << "Transmit/Receive: " << static_cast<int>(command.transmit) << endl;
    cout << "Sub Address: " << static_cast<int>(command.subAddress) << endl;
    cout << "Word Count: " << static_cast<int>(command.wordCount) << endl;
}

void RemoteTerminal::ReceiveData(int wordCount,int subAddress) {
    for (int i = 0;i < wordCount;i++) {
        int DataReceived = recvfrom(sock, reinterpret_cast<char*>(&DataWord), sizeof(DataWord), 0,
            reinterpret_cast<sockaddr*>(&sender),
            &senderSize);

        if (subAddress + i > memory.size()) {
            messageerror = true;
        }
        else {
        memory[subAddress + i] = DataWord;
        }

        if (DataReceived == SOCKET_ERROR){
            cout << "Receive failed: " << WSAGetLastError() << endl;
            return;
        }
        else {
            cout << "Data " << memory[subAddress + i] << " Written on the address: " << subAddress + i << endl;
        }
    }
}

void RemoteTerminal::SendData(int wordCount, int subAddress) {
    uint16_t data;
    for (int i = 0;i < wordCount;i++) {
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
        if (static_cast<int>(command.rtAddress) == 5) {
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