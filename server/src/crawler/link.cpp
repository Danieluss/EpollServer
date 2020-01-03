#include"crawler/link.hpp"
#include<regex>
#include<assert.h>
#include<iostream>

Link::Link(string url) : url(url) {
    depth = compute_depth();
}

int Link::compute_depth() {
    regex r("https?://[A-Za-z\\.]+(.*)");
    smatch m;
    assert(regex_match(url, m, r));
    string s = m[1].str();
    int res=0;
    for(char c : s) {
        if(c == '/') {
            res++;
        }
    }
    return res;
}

bool operator<(const Link &l1, const Link &l2) {
    return l1.depth > l2.depth;
}
