#include "service/dynamic_thread_pool_executor.h"
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    DynamicThreadPool poolExecutor(3, 8);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            poolExecutor.enqueue_task([]() -> void {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                std::cout<<"inside job"<<std::endl;
            });
        }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1200));
    poolExecutor.finish();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}