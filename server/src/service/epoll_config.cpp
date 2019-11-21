#include "service/epoll_config.h"

EpollConfig::EpollConfig(std::string host_addr, int port, int backlog,
                         std::function<std::string(std::string)> get_response, int max_events)
        : host_addr(host_addr), port(port), backlog(backlog), get_response(get_response), max_events(max_events) {
}