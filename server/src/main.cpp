#include <iostream>
#include <chrono>
#include <thread>
#include <bits/stdc++.h>
#include "service/epoll_server.h"
#include "multi_threading/synch_logger.h"
#include "service/server_thread_pool_manager.h"

int main(int argc, char** argv) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    ServerThreadPoolManager server({"0.0.0.0", 10101, 128, [](std::string req) -> std::string { return "http://google.pl/;some_title\nhttp://localhost:8080/;some_title\n"; },
                                    64});
    return 0;
}