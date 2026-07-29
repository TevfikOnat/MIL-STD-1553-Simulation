#include "BusController.h"
#include "Ports.h"

using namespace std;

void BusController::InitializeSocket() {
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
    address.sin_port = htons(Ports::BusController);
    address.sin_addr.s_addr = INADDR_ANY;

    destination.sin_family = AF_INET;
    destination.sin_port = htons(Ports::BUS);
    destination.sin_addr.s_addr = INADDR_ANY;
    InetPton(AF_INET, "127.0.0.1", &destination.sin_addr);

    if (bind(sock, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == SOCKET_ERROR) {
        cout << "Bind failed: " << WSAGetLastError() << endl;
    }    
}

void BusController::CloseSocket(SOCKET sock) {
	closesocket(sock);
	WSACleanup();
}

void BusController::SendCommand() {
    uint16_t cmd = EncodeCommandWord();
    decodedcmd = DecodeCMD(cmd);

    int CMDSent = sendto(sock, reinterpret_cast<char*>(&cmd), sizeof(cmd), 0,
        reinterpret_cast<sockaddr*>(&destination),
        sizeof(destination));

    if (CMDSent == SOCKET_ERROR)
    {
        cout << "Send failed: " << WSAGetLastError() << endl;
    }
}

void BusController::SendData(DecodedCommand decodedcmd) {
    for (int i = 0;i < decodedcmd.wordCount;i++) {
        uint16_t data = 0;
        cout << "Data to send to the address " << decodedcmd.rtAddress + i << ": ";
        cin >> data;
        int DataSent = sendto(sock, reinterpret_cast<char*>(&data), sizeof(data), 0,
            reinterpret_cast<sockaddr*>(&destination),
            sizeof(destination));
    }           
}

void BusController::ReceiveData() {
	uint16_t DataWord;
	int DataReceived;

    for (int i = 0;i < decodedcmd.wordCount;i++) {
        DataReceived = recvfrom(sock, reinterpret_cast<char*>(&DataWord), sizeof(DataWord), 0,
            reinterpret_cast<sockaddr*>(&destination), &destinationSize);

        if (DataReceived == SOCKET_ERROR) {
            cout << "Receive failed: " << WSAGetLastError() << endl;
        }
        else {
            cout << "Received data: " << DataWord << endl;
        }
    }
}

DecodedStatus BusController::ReceiveStatus() {
	uint16_t StatusWord;
    int StatusReceived = recvfrom(sock, reinterpret_cast<char*>(&StatusWord), sizeof(StatusWord), 0,
        reinterpret_cast<sockaddr*>(&destination), &destinationSize);

    if (StatusReceived == SOCKET_ERROR) {
        cout << "Receive failed: " << WSAGetLastError() << endl;
    }

    DecodedStatus decodedstat = DecodeStatus(StatusWord);

    if (decodedstat.messageError == 1 || decodedstat.busy == 1) {
        cout << "RT reported an error" << endl;
    }
	
	return decodedstat;
}

void BusController::Run() {
	while (true) {
        SendCommand();
        if (decodedcmd.transmit == 0) {
            SendData(decodedcmd);
            ReceiveStatus();
        }
        else if (decodedcmd.transmit == 1) {
            ReceiveStatus();
            ReceiveData();
        }
	}    
}

BusController::BusController() {
    InitializeSocket();
}

BusController::~BusController() {
    CloseSocket(sock);
}

