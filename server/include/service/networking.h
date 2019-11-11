#ifndef SEARCHENGINE_NETWORKING_H
#define SEARCHENGINE_NETWORKING_H

#include <sys/select.h>
#include <service/dynamic_thread_pool_executor.h>

class RequestDispatcher {

    int listen_fd;
    fd_set ready_set;
    DynamicThreadPool thread_pool = DynamicThreadPool(3);

public:

    RequestDispatcher(const char* ip, int port, int backlog);

    void run();

};

#endif //SEARCHENGINE_NETWORKING_H