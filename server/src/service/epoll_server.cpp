#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include "service/epoll_server.h"
#include "multi_threading/synch_logger.h"

#define ADDR_BUFFER_SIZE 64
#define PORT_BUFFER_SIZE 8
#define REQUEST_CHUNK_SIZE 1024
#define REQUEST_HEADER_MAX_SIZE 16

std::mutex mutex;

EpollServer::EpollServer(EpollConfig config)
        : port(config.port), backlog(config.backlog), host_addr(config.host_addr), get_response(config.get_response),
          max_events(config.max_events) {
}

void EpollServer::init() {
    struct sockaddr_in server_addr;
    struct epoll_event event;
    struct hostent *host;
    int options = 1;

    std::unique_lock lock(mutex);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        synch_log.perror("socket()");
        exit(1);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &options, sizeof(int)) == -1) {
        synch_log.perror("setsockopt()");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    host = gethostbyname(host_addr.c_str());
    memcpy(&server_addr.sin_addr.s_addr, host->h_addr, host->h_length);

    bzero(&(server_addr.sin_zero), 8);

    if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr)) == -1) {
        synch_log.perror("bind()");
        exit(1);
    }

    make_socket_non_blocking(server_fd);

    if (listen(server_fd, backlog) == -1) {
        synch_log.perror("listen()");
        exit(1);
    }

    synch_log.print("Waiting for connections on %s:%d...\n", host_addr.c_str(), port);

    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        synch_log.perror("epoll_create1()");
        exit(1);
    }

    event.data.fd = server_fd;
    event.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
        synch_log.perror("epoll_ctl()");
        exit(1);
    }

    events = (epoll_event *) calloc(max_events, sizeof(event));
}

void EpollServer::make_socket_non_blocking(int sfd) {
    int flags;

    flags = fcntl(sfd, F_GETFL, 0);
    if (flags == -1) {
        synch_log.perror("fcntl(F_GETFL)");
        exit(1);
    }

    flags |= O_NONBLOCK;
    if (fcntl(sfd, F_SETFL, flags) == -1) {
        synch_log.perror("fcntl(F_SETFL)");
        exit(1);
    }
}

void EpollServer::accept_connection() {
    struct epoll_event event;
    struct sockaddr connection_addr;
    socklen_t connection_len = sizeof(connection_addr);
    int connection_fd;
    char port_buff[PORT_BUFFER_SIZE], addr_buff[ADDR_BUFFER_SIZE];

    while ((connection_fd = accept(this->server_fd, &connection_addr, &connection_len)) != -1) {

        if (getnameinfo(&connection_addr, connection_len,
                        addr_buff, sizeof(addr_buff),
                        port_buff, sizeof(port_buff),
                        NI_NUMERICHOST) == 0) {
            synch_log.print("Accepted connection from %s:%s\n",
                            addr_buff, port_buff);
        }

        make_socket_non_blocking(connection_fd);

        event.data.fd = connection_fd;
        event.events = EPOLLIN | EPOLLET;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, connection_fd, &event) == -1) {
            synch_log.perror("epoll_ctl()");
            exit(1);
        }
        connection_len = sizeof(connection_addr);
    }

    if (errno != EAGAIN && errno != EWOULDBLOCK) {
        synch_log.perror("there was a problem when accepting connection");
    }
}

bool EpollServer::request_map_contains(int fd) {
    return fd_request_map.find(fd) != fd_request_map.end();
}

void EpollServer::forge_response(int connection_fd) {
    ssize_t count;
    char request_buffer[REQUEST_CHUNK_SIZE];
    std::string request_chunk = "";
    struct epoll_event event;

    while ((count = read(connection_fd, request_buffer, sizeof(request_buffer) - 1))) {
        if (count == -1) {
            if (errno == EAGAIN) {
                break;
            }

            synch_log.perror("read()");
            break;
        }
        request_buffer[count] = '\0';
        request_chunk.append(std::string(request_buffer));
    }

    if (request_map_contains(connection_fd)) {
        fd_request_map[connection_fd] = fd_request_map[connection_fd] + request_chunk;
    } else {
        fd_request_map.insert({connection_fd, request_chunk});
    }

    if(fd_request_map[connection_fd].back() == '\n') {
        fd_response_map[connection_fd] = get_response(fd_request_map[connection_fd]);
        fd_pending_response_map[connection_fd] = (char*) fd_response_map[connection_fd].c_str();
        event.data.fd = connection_fd;
        event.events = EPOLLOUT | EPOLLET;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, connection_fd, &event) == -1) {
            synch_log.perror("epoll_ctl()");
            exit(1);
        }
        synch_log.print("Forged response for connection no %d\n", connection_fd);
    }
}

void EpollServer::respond(int connection_fd) {
    char* response = fd_pending_response_map[connection_fd];
    int response_count;
    if ((response_count = write(connection_fd, response, strlen(response))) == -1) {
        synch_log.perror("write()");
    }
    fd_pending_response_map[connection_fd] = response + response_count;
    if (fd_pending_response_map[connection_fd][0] == '\0') {
        close(connection_fd);
        synch_log.print("Responded to connection %d with \n------\n%s\n------\n", connection_fd,
                        fd_response_map[connection_fd].c_str());
        fd_request_map.erase(connection_fd);
        fd_response_map.erase(connection_fd);
        fd_pending_response_map.erase(connection_fd);
    }
}

void EpollServer::await_requests() {
    int n, i;
    n = epoll_wait(epoll_fd, events, max_events, 200);
    for (i = 0; i < n; i++) {
        if (events[i].events & EPOLLERR || events[i].events & EPOLLHUP ||
            (!(events[i].events & EPOLLIN) && !(events[i].events & EPOLLOUT))) {
            synch_log.perror("epoll_wait()");
            close(events[i].data.fd);
        } else {
            if (events[i].data.fd == server_fd) {
                accept_connection();
            } else {
                if (events[i].events & EPOLLIN) {
                    forge_response(events[i].data.fd);
                }
                if (events[i].events & EPOLLOUT) {
                    respond(events[i].data.fd);
                }
            }
        }
    }
}

void EpollServer::run() {
    synch_log.print("Starting...\n");
    init();
    synch_log.print("Initialized\n");

    while (alive.read()) {
        await_requests();
    }

    free(events);
    close(server_fd);
    synch_log.print("Closed server thread\n");
}

void EpollServer::stop() {
    alive.write(false);
}

void EpollServer::run_asynch() {
    thread = std::thread(&EpollServer::run, this);
}