#include"crawler/link.h"
#include<regex>
#include<assert.h>
#include<iostream>

Link::Link(string url) : url(url) {
    depth = compute_depth();
}

int Link::compute_depth() {
    regex r("https?://([A-Za-z\\.]+)(.*)");
    smatch m;
    if (!regex_match(url, m, r)) {
        cerr << "can't compute depth " << url << "\n";
        return 100;
    }
    int res = 0;
    string s = m[1].str();
    for(char c : s) {
        if(c == '.') {
            res++;
        }
    }
    s = m[2].str();
    if((int)s.size() > 1) {
        res+=2;
    }
    for (char c : s) {
        if (c == '/') {
            res+=2;
        }
    }
    return res;
}

bool operator<(const Link &l1, const Link &l2) {
    return l1.depth > l2.depth;
}
