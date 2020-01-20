#include"crawler/crawler.h"
#include"utils.h"

void Crawler::save() {
    storage.save();
    vector <string> linksVector;
    while (!links.empty()) {
        linksVector.push_back(links.top().url);
        links.pop();
    }
    writeObjectToFile(linksFile, linksVector);
    writeObjectToFile(visitedFile, visited);
}

void Crawler::load() {
    vector <string> linksVector;
    readObjectFromFile(linksFile, linksVector);
    readObjectFromFile(visitedFile, visited);
    cerr << SIZE(linksVector) << "\n";
    for (string &s : linksVector) {
        links.push(Link(s));
    }
}

Crawler::Crawler() {
    load();
}

void Crawler::updateLinks(vector <string> links) {
    {
        lock_guard<mutex> lock(linksMutex);
        for (string l : links) {
            if (!visited.count(l)) {
                visited.insert(l);
                this->links.push(Link(l));
            }
        }
    }
    cv.notify_all();
}

Link Crawler::getNextLink() {
    unique_lock<mutex> lock(linksMutex);
    cv.wait(lock, [&](){ return !links.empty(); });
    Link l = links.top();
    links.pop();
    cerr << "Links size " << SIZE(links) << "\n";
    return l;
}

void Crawler::runSingleThread() {
    while (true) {
        Link l = getNextLink();

        string html;
        int code;
        try {
            auto res = htmlFetcher.fetch(l.url);
            html = res.first;
            code = res.second;
        } catch (runtime_error &re) {
            cerr << re.what() << "\n";
            continue;
        }

        HTMLParser htmlParser(html, l.url);
        htmlParser.parse();
        htmlParser.report();
        updateLinks(htmlParser.getLinks());

        if (code < 300) {
            lock_guard<mutex> lock(storageMutex);
            cerr << "adding\n";
            storage.add(htmlParser);
        }
        {
            lock_guard<mutex> lock(limitMutex);
            limit--;
            cerr << "Left: " << limit << "\n";
            if (limit < 0) {
                break;
            }
        }
    }
}

void Crawler::run(int limit, int numThreads) {
    this->limit = limit;
    vector<thread> threads;
    for(int i=0; i < numThreads; i++) {
        threads.push_back(thread(&Crawler::runSingleThread, this));
    }
    for(int i=0; i < numThreads; i++) {
        threads[i].join();
    }
    save();
}