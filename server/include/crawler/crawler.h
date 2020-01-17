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
    int limit;
    mutex storageMutex, linksMutex, limitMutex;
    condition_variable cv;

    void save();

    void load();

    void runSingleThread();

    Link getNextLink();

public:
    Crawler();

    void updateLinks(vector <string> links);

    void run(int limit, int numThreads);
};

#endif //CRAWLER_H
