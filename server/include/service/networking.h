#ifndef SEARCHENGINE_NETWORKING_H
#define SEARCHENGINE_NETWORKING_H

#include <sys/select.h>

class RequestDispatcher {

    int listen_fd;
    fd_set ready_set;

public:

    RequestDispatcher(const char* ip, int port);

    void run();

};

#endif //SEARCHENGINE_NETWORKING_H