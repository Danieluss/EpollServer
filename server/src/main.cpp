#include <iostream>
#include <chrono>
#include <thread>
#include <bits/stdc++.h>
#include <unistd.h>
#include "service/epoll_server.h"
#include "multi_threading/synch_logger.h"
#include "service/server_thread_pool_manager.h"
#include"crawler/crawler.hpp"


string read_file(string filename) {
    ifstream in(filename, ios::binary);
    string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return contents;
}

int main(int argc, char** argv) {
    std::string usage_string = "Usage: bash -c main.out { populate | host } { $1==populate ? start_url : } { $1==populate ? limit_int : }";
    std::string wrong_request_string = "#ERROR\nMalformed request\n";
    if(argc < 4) {
        std::cerr<<usage_string<<std::endl;
        return -1;
    }
    std::string second_arg = std::string(argv[2]);
    int third_arg = std::stoi(argv[3]);
    if(std::string(argv[1]) == "host") {
        std::ios_base::sync_with_stdio(false);
        std::cin.tie(NULL);
        Storage storage;
        ServerThreadPoolManager server({second_arg, third_arg, 128,
            [&storage](std::string req) -> std::string {
                int page, int page_size;
                istringstream iss(req);
                iss >> page >> page_size;
            },
            64});
    } else if(std::string(argv[1]) == "populate") {
        if (argc < 4) {
            std::cerr<<usage_string<<std::endl;
            return -1;
        }
        Crawler crawler;
        crawler.updateLinks({second_arg});
        crawler.run(third_arg);
    } else {
        std::cerr<<usage_string<<std::endl;
        return -1;
    }
    return 0;
}