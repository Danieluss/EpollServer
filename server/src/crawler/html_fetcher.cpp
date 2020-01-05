#include"crawler/html_fetcher.hpp"

HTMLFetcher::HTMLFetcher() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

HTMLFetcher::~HTMLFetcher() {
    curl_global_cleanup();
}

size_t HTMLFetcher::writeToString(char *buf, size_t size, size_t nmemb, string *data) {
    size_t n = size*nmemb;
    for(int i=0;i < (int)n; i++) {
        (*data)+=buf[i];
    }
    return n;
}

pair<string, long> HTMLFetcher::fetch(string url) {
    CURL *curl;
    CURLcode res;
    string response;

    curl = curl_easy_init();
    if(!curl) {
        throw(runtime_error("Cannot init curl"));
    }
    curl_easy_setopt(curl, CURLOPT_URL, url.data());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
    res = curl_easy_perform(curl);
    if(res != CURLE_OK) {
        curl_easy_cleanup(curl);
        throw(runtime_error("Curl error: " + string(curl_easy_strerror(res))));
    }

    long code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
    if(code >= 400) {
        curl_easy_cleanup(curl);
        throw(runtime_error("HTTP response code " + to_string(code)));
    }

    curl_easy_cleanup(curl);

    return {response, code};
}