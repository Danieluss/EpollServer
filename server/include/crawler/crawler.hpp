#ifndef CRAWLER_HPP
#define CRAWLER_HPP

#include"storage/storage.hpp"
#include"crawler/html_parser.hpp"
#include"crawler/html_fetcher.hpp"
#include"crawler/link.hpp"

struct Crawler {
private:
    Storage storage;
    HTMLFetcher htmlFetcher;
    priority_queue<Link> links;
    unordered_set<string> visited;
public:
    void updateLinks(vector<string> links);
    void run();
};

#endif //CRAWLER_HPP
