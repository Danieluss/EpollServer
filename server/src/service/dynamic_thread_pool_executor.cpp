#include "service/dynamic_thread_pool_executor.h"
#include <iostream>

DynamicThreadPool::DynamicThreadPool(int lower_bound, int upper_bound) {
    this->lower_bound = lower_bound;
    this->upper_bound = upper_bound;
    grow(lower_bound);
}

void DynamicThreadPool::worker_thread() {
    while (true) {
        std::function<void()> task;
        {
            ready_threads++;
            std::unique_lock<decltype(mutex)> lock(mutex);
            if (tasks.empty() && kill_next == 0) {
                std::cout<<std::this_thread::get_id()<<" is sleeping"<<std::endl;
                cond_var.wait(lock, [&] { return !tasks.empty() || kill_next > 0; });
                std::cout<<std::this_thread::get_id()<<" woke up"<<std::endl;
            }
            if (kill_next > 0) {
                kill_next--;
                curr_threads--;
                ready_threads--;
                break;
            }
            task = std::move(tasks.front());
            tasks.pop_front();
            ready_threads--;
        }
        task();
        std::cout<<std::this_thread::get_id()<<" executed a request "<<std::endl;
    }
    std::cout<<"Thread "<<std::this_thread::get_id()<<" is dying"<<std::endl;
}

void DynamicThreadPool::grow(int n_threads) {
    {
        std::lock_guard<decltype(mutex)> lock(mutex);
        int max = upper_bound - curr_threads;
        if (n_threads > max) {
            n_threads = max;
        }
        if (!finished && n_threads > 0) {
            if (kill_next > 0) {
                kill_next -= n_threads;
                if (kill_next < 0) {
                    n_threads = -kill_next;
                    kill_next = 0;
                } else {
                    return;
                }
            }
            for (int i = 0; i < n_threads; ++i) {
                std::thread thread(&DynamicThreadPool::worker_thread, this);
                std::cout<<"Creating "<<thread.get_id()<<std::endl;
                thread.detach();
            }
            curr_threads += n_threads;
        }
    }
}

void DynamicThreadPool::shrink(int n_threads) {
    {
        std::lock_guard<decltype(mutex)> lock(mutex);
        int min = curr_threads - lower_bound;
        if (n_threads > min) {
            n_threads = min;
        }
        if (n_threads > 0) {
            std::cout<<"Ordering "<<n_threads<<" to be killed"<<std::endl;
            kill_next += n_threads;
        }
    }
    cond_var.notify_all();
}

void DynamicThreadPool::enqueue_task(std::function<void()> task) {
    std::cout<<"Enqueuing task"<<std::endl;
    {
        std::lock_guard<decltype(mutex)> lock(mutex);
        tasks.emplace_back(task);
        std::cout<<curr_threads<<" "<<ready_threads<<"<<<<<<<<<<"<<std::endl;
        if (ready_threads == 0) {
           grow(1);
        }
    }
    cond_var.notify_one();
}

void DynamicThreadPool::cancel_pending() {
    {
        std::lock_guard<decltype(mutex)> lock(mutex);
        tasks.clear();
    }
    std::cout<<"Cancelling pending requests "<<std::endl;
}

void DynamicThreadPool::finish() {
    {
        std::lock_guard<decltype(mutex)> lock(mutex);
        tasks.clear();
        kill_next = curr_threads;
        finished = true;
    }
    cond_var.notify_all();
    std::cout<<"Killing all threads"<<std::endl;
}

DynamicThreadPool::~DynamicThreadPool() {
    finish();
    std::cout<<"Destructing DynamicThreadPool"<<std::endl;
}