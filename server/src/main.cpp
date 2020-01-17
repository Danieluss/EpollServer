#include <iostream>
#include <chrono>
#include <thread>
#include <bits/stdc++.h>
#include <unistd.h>
#include "service/epoll_server.h"
#include "multi_threading/synch_logger.h"
#include "service/server_thread_pool_manager.h"
#include "crawler/crawler.h"
#include "utils.h"


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

int main(int argc, char **argv) {
    std::string usage_string = "Usage: bash -c main.out { populate | host } { $1==populate ? start_url : ip } { $1==populate ? limit_int : port } { $1==populate ? number_of_threads : }";
    std::string wrong_request_string = "#ERROR\nMalformed request\n";
    if (argc < 4) {
        std::cerr << usage_string << std::endl;
        return -1;
    }
    std::string second_arg = std::string(argv[2]);
    int third_arg = std::stoi(argv[3]);
    if (std::string(argv[1]) == "host") {
        std::ios_base::sync_with_stdio(false);
        std::cin.tie(NULL);
        Storage storage;
        ServerThreadPoolManager server({second_arg, third_arg, 128,
                                        [&storage, &wrong_request_string](std::string req) -> std::string {
                                            try {
                                                int page;
                                                int page_size;
                                                istringstream iss(req);
                                                iss >> page >> page_size;
                                                pair<vector<Entry>, int> result = storage.query(page, page_size,
                                                                                                str_split(req.substr(iss.tellg())));
                                                std::ostringstream response;
                                                response << "#OK-\n" << result.second << "-\n";
                                                for (Entry entry : result.first) {
                                                    response << entry.url << ";\n";
                                                    response << entry.title << ";\n";
                                                    response << entry.description << ";\n";
                                                    response << "\0";
                                                }
                                                return response.str();
                                            } catch (exception& e) {
                                                return wrong_request_string;
                                            }
                                        },
                                        64});
    } else if (std::string(argv[1]) == "populate") {
        if (argc < 5) {
            std::cerr << usage_string << std::endl;
            return -1;
        }
        int fourth_arg = stoi(argv[4]);
        Crawler crawler;
        crawler.updateLinks({second_arg});
        crawler.run(third_arg, fourth_arg);
    } else {
        std::cerr << usage_string << std::endl;
        return -1;
    }
    return 0;
}