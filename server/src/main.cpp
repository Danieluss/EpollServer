#include <iostream>
#include <chrono>
#include <thread>
#include <bits/stdc++.h>
#include <unistd.h>
#include "service/epoll_server.h"
#include "multi_threading/synch_logger.h"
#include "service/server_thread_pool_manager.h"

// int main(int argc, char** argv) {
//     std::ios_base::sync_with_stdio(false);
//     std::cin.tie(NULL);
//     ServerThreadPoolManager server({"0.0.0.0", 10101, 128, [](std::string req) -> std::string { return "http://google.pl/;some_title\nhttp://localhost:8080/;some_title\n"; },
//                                     64});
//     return 0;
// }

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

int main() {
    // Document d("ĄBCeeeFGąęśżźćńóĄĘŚŻŹĆŃÓ\nabc äöüß\nÄÖÜß–\ntak", "ssss", "ĄBCeeeFGąęśżźćńóĄĘŚŻŹĆŃÓ\nabc äöüß\nÄÖÜß–\ntak");
    // cerr << d.title << "\n" << d.description << "\n";
    // Crawler crawler;
    // crawler.updateLinks({"https://skijumping.pl/"});
    // crawler.run(10);
    Storage storage;
    auto res = storage.query(0, 4, "software");
    vector<Entry> entries = res.first;
    for(Entry &e : entries) {
        cerr << e << "\n";
    }
    cerr << res.second << "\n";
}