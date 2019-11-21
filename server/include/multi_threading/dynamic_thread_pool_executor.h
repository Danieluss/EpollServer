#ifndef SEARCHENGINE_DYNAMIC_THREAD_POOL_EXECUTOR_H
#define SEARCHENGINE_DYNAMIC_THREAD_POOL_EXECUTOR_H

#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <vector>
#include "utils.h"


class DynamicThreadPoolExecutor {

    volatile int lower_bound;
    volatile int upper_bound;
    volatile int ready_threads = 0;
    volatile bool finished = false;
    volatile int kill_next = 0;
    volatile int curr_threads = 0;

    std::recursive_mutex mutex;
    std::condition_variable_any cond_var;
    std::deque<std::function<void()>> tasks;

    void worker_thread();

public:

    DynamicThreadPoolExecutor(int lower_bound = 1, int upper_bound = std::thread::hardware_concurrency());

    void grow(int n_threads);

    void shrink(int n_threads);

    void enqueue_task(std::function<void()> task);

    void cancel_pending();

    void finish();

    virtual ~DynamicThreadPoolExecutor();

};

#endif //SEARCHENGINE_DYNAMIC_THREAD_POOL_EXECUTOR_H
