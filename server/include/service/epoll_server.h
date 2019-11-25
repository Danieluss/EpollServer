#ifndef SEARCHENGINE_EPOLL_SERVER_H
#define SEARCHENGINE_EPOLL_SERVER_H

#include <string>
#include <functional>
#include <thread>
#include <unordered_map>
#include <sys/epoll.h>
#include <poll.h>
#include "multi_threading/synch_counter.h"
#include "multi_threading/synch_resource.h"
#include "service/epoll_config.h"



class EpollServer {

    int port, backlog, epoll_fd, server_fd;
    std::string host_addr;
    std::unordered_map<int, std::string> fd_request_map;
    std::unordered_map<int, std::string> fd_response_map;
    std::unordered_map<int, char*> fd_pending_response_map;
    std::function<std::string(std::string)> get_response;
    struct epoll_event *events;
    SynchResource<bool> alive{true};
    int max_events;
    std::thread thread;

    void init();

    void make_socket_non_blocking(int sfd);

    void await_requests();

    void accept_connection();

    void forge_response(int connection_fd);

    bool request_map_contains(int fd);

    void respond(int connection_fd);

public:

    EpollServer(EpollConfig config);

    void run();

    void run_asynch();

    void stop();


};


#endif //SEARCHENGINE_EPOLL_SERVER_H
