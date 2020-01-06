#ifndef SEARCHENGINE_DYNAMICTHREADPOOLSERVER_H
#define SEARCHENGINE_DYNAMICTHREADPOOLSERVER_H

#include <thread>
#include <vector>
#include "service/epoll_server.h"

class ServerThreadPoolManager {

    EpollConfig config;
    volatile int lower_bound;
    volatile int upper_bound;
    std::vector<EpollServer *> servers;

    void grow(int n_threads);

    void _shrink(int n_threads);

    void shrink(int n_threads);

    void run();

public:

    ServerThreadPoolManager(EpollConfig epollConfig, int lower_bound = std::thread::hardware_concurrency(),
                            int upper_bound = std::thread::hardware_concurrency());

    void finish();

    virtual ~ServerThreadPoolManager();

};


#endif //SEARCHENGINE_DYNAMICTHREADPOOLSERVER_H
