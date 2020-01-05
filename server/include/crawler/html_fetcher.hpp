#ifndef HTML_FETCHER_HPP
#define HTML_FETCHER_HPP

#include<bits/stdc++.h>
#include<curl/curl.h>
using namespace std;

struct HTMLFetcher {
private:
    static size_t writeToString(char *buf, size_t size, size_t nmemb, string *data);
public:
    HTMLFetcher();
    ~HTMLFetcher();
    pair<string, long> fetch(string url);
};

#endif //HTML_FETCHER_HPP
