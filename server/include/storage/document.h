#ifndef DOCUMENT_H
#define DOCUMENT_H

#include<bits/stdc++.h>

using namespace std;

struct Document {
    string title;
    string url;
    string description;
    int wordcount;
    unordered_map<int, int> terms;

    Document();

    Document(string title, string url, string description);

    string sanitizeTitle(string s);
};

ostream &operator<<(ostream &stream, Document &d);

istream &operator>>(istream &stream, Document &d);

#endif //DOCUMENT_H
