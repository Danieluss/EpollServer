#include "service/server_thread_pool_manager.h"
#include <unistd.h>

void ServerThreadPoolManager::grow(int n_threads) {
    if (n_threads > upper_bound) {
        n_threads = upper_bound;
    }
    for (int i = servers.size(); i < n_threads; i++) {
        servers.push_back(new EpollServer(config));
        servers.back()->run_asynch();
    }
}

void ServerThreadPoolManager::_shrink(int n_threads) {
    while(servers.size() > (unsigned) n_threads) {
        EpollServer* server = servers.back();
        servers.pop_back();
        server->stop();
    }
}

void ServerThreadPoolManager::shrink(int n_threads) {
    if (n_threads < lower_bound) {
        n_threads = lower_bound;
    }
    shrink(n_threads);
}


ServerThreadPoolManager::ServerThreadPoolManager(EpollConfig epollConfig, int lower_bound,
                                                 int upper_bound) : config(epollConfig), lower_bound(lower_bound), upper_bound(upper_bound) {
    grow(lower_bound);
    run();
}

void ServerThreadPoolManager::finish() {
    _shrink(0);
}

void ServerThreadPoolManager::run() {
    while(true) {
        sleep(1);
    }
}

ServerThreadPoolManager::~ServerThreadPoolManager() {
    finish();
}