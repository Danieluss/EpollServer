#ifndef CRAWLER_H
#define CRAWLER_H

#include"storage/storage.h"
#include"crawler/html_parser.h"
#include"crawler/html_fetcher.h"
#include"crawler/link.h"

struct Crawler {
private:
    Storage storage;
    HTMLFetcher htmlFetcher;
    priority_queue <Link> links;
    unordered_set <string> visited;
    string linksFile = "links";
    string visitedFile = "visited";

    void save();

    void load();

public:
    Crawler();

    void updateLinks(vector <string> links);

    void run(int limit);
};

#endif //CRAWLER_H
