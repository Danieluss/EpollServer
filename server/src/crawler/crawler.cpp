#include"crawler/crawler.hpp"

void Crawler::updateLinks(vector<string> links) {
    for(string l : links) {
        if(!visited.count(l)) {
            visited.insert(l);
            this->links.push(Link(l));
        }
    }
}

void Crawler::run() {
    int counter=0;
    while(!links.empty()) {
        Link l = links.top();
        links.pop();

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
        if(counter > 50) {
            break;
        }
    }
    //TODO save storage, links and visited
}