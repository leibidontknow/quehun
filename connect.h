// server.cpp
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "type.h"
#include "game.h"
void connect(){
    int server_fd, new_socket[4];
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // 创建 socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 绑定 socket
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 监听
    if (listen(server_fd, 10) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Waiting for connections on port " << PORT << "...\n";

    // 接受连接
    if ((new_socket[0] = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    if ((new_socket[1] = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    if ((new_socket[2] = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    if ((new_socket[3] = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    send(new_socket[0], "OK", 2, 0);
    send(new_socket[1], "OK", 2, 0);
    send(new_socket[2], "OK", 2, 0);
    send(new_socket[3], "OK", 2, 0);
    game(new_socket);

    // 关闭 socket
    close(new_socket[0]);
    close(new_socket[1]);
    close(new_socket[2]);
    close(new_socket[3]);
    close(server_fd);
}
