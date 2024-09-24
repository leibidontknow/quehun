#include <bits/stdc++.h>
#include <winsock2.h>
#include <thread>
#include "type.h"
using namespace std;
#pragma comment(lib, "Ws2_32.lib")
SOCKET sock;
void send(const char *BUF, int siz){
    send(sock, BUF, siz, 0);
}
int get(char *BUF){
    int res = recv(sock, BUF, sizeof(BUF), 0);
    return res;
}
void connect(){    // 初始化 Winsock
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
            return;
        }

        // 创建 socket
        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sock == INVALID_SOCKET) {
            std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return;
        }

        // 设置服务器地址
        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(11451); // 端口号
        serverAddr.sin_addr.s_addr = inet_addr("119.91.38.43"); // IP 地址

        // 连接到服务器
        if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "Connection failed: " << WSAGetLastError() << std::endl;
            closesocket(sock);
            WSACleanup();
            return;
        }

        std::cout << "Connected to the server successfully!" << std::endl;
}
void close(){
    closesocket(sock);
    WSACleanup();
}