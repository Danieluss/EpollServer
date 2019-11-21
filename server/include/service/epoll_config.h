#ifndef SEARCHENGINE_EPOLL_CONFIG_H
#define SEARCHENGINE_EPOLL_CONFIG_H

#include <functional>
#include <string>
#include "multi_threading/synch_counter.h"
#include "multi_threading/synch_resource.h"

struct EpollConfig {
    std::string host_addr;
    int port;
    int backlog;
    std::function<std::string(std::string)> get_response;
    int max_events;

    EpollConfig(std::string host_addr, int port, int backlog, std::function<std::string(std::string)> get_response, int max_events);
};

#endif //SEARCHENGINE_EPOLL_CONFIG_H
