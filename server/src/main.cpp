#include <iostream>
#include <chrono>
#include <thread>
#include "service/dynamic_thread_pool_executor.h"
#include "service/networking.h"


int main() {
    RequestDispatcher requestDispatcher("0.0.0.0", 10101, 64);
    requestDispatcher.run();
}