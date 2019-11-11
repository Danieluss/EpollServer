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
#include "service/networking.h"

RequestDispatcher::RequestDispatcher(const char* ip, int port, int backlog) {
    struct sockaddr_in server_addr;
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(ip);
    server_addr.sin_port = htons(port);

    bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    FD_ZERO(&ready_set);

    listen(listen_fd, backlog);
}

void RequestDispatcher::run() {
    int connection_fd, n_ready;
    char buffer[MAXLINE];
    ssize_t n;
    socklen_t len;
    struct sockaddr_in cliaddr;
    struct timeval  timeout;

    timeout.tv_sec = WAIT;
    timeout.tv_usec = 0;
    while (true) {

        FD_SET(listen_fd, &ready_set);
        n_ready = select(listen_fd + 1, &ready_set, NULL, NULL, timeout);

        if (FD_ISSET(listen_fd, &ready_set)) {
            len = sizeof(cliaddr);
            connection_fd = accept(listen_fd, (struct sockaddr*)&cliaddr, &len);
//            TODO
            close(connfd);
        } else {
            std::cout<<"Timeout"<<std::endl;
        }
    }
}