#include"crawler/crawler.hpp"
#include"utils.h"
#include"utils.cpp"

void Crawler::save() {
    storage.save();
    vector<string> linksVector;
    while(!links.empty()) {
        linksVector.push_back(links.top().url);
        links.pop();
    }
    writeObjectToFile(linksFile, linksVector);
    writeObjectToFile(visitedFile, visited);
}

void Crawler::load() {
    vector<string> linksVector;
    writeObjectToFile(linksFile, linksVector);
    writeObjectToFile(visitedFile, visited);
    for(string &s : linksVector) {
        links.push(Link(s));
    }
}

Crawler::Crawler() {
    load();
}

void Crawler::updateLinks(vector<string> links) {
    for(string l : links) {
        if(!visited.count(l)) {
            visited.insert(l);
            this->links.push(Link(l));
        }
    }
}

void Crawler::run(int limit) {
    int counter=0;
    while(!links.empty()) {
        Link l = links.top();
        links.pop();
        cerr << "Links size " << SIZE(links) << "\n";

        string html;
        int code;
        try {
            auto res = htmlFetcher.fetch(l.url);
            html = res.first;
            code = res.second;
        } catch(runtime_error &re) {
            cerr << re.what() << "\n";
            continue;
        }

        HTMLParser htmlParser(html, l.url);
        htmlParser.parse();
        htmlParser.report();
        updateLinks(htmlParser.getLinks());

        if(code < 300) {
            cerr << "adding\n";
            storage.add(htmlParser);
        }
        counter++;
        if(counter > limit) {
            break;
        }
    }
    save();
}