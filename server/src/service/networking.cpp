#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include "service/networking.h"
#define MAX_LINE 1024

RequestDispatcher::RequestDispatcher(const char* ip, int port, int backlog) {
    struct sockaddr_in server_addr;
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(inet_addr(ip));
    server_addr.sin_port = htons(port);

    bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    FD_ZERO(&ready_set);

    listen(listen_fd, backlog);
}

void RequestDispatcher::run() {
    int connection_fd, n_ready;
    socklen_t len;
    struct sockaddr_in client_addr;
    struct timeval  timeout;

    while (true) {
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;

        FD_SET(listen_fd, &ready_set);
        n_ready = select(listen_fd + 1, &ready_set, NULL, NULL, &timeout);

        if (FD_ISSET(listen_fd, &ready_set)) {
            len = sizeof(client_addr);
            connection_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &len);
            thread_pool.enqueue_task([connection_fd]()->void{
                std::string mock_answer = "Server responded with title<\n>www.google.pl<\t\n>";
                write(connection_fd, mock_answer.c_str(), mock_answer.length() + 1);
            });
//            TODO
            close(connection_fd);
        } else {
            std::cout<<"No connection in 10s"<<std::endl;
        }
    }
}