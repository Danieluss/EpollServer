#ifndef DOCUMENT_HPP
#define DOCUMENT_HPP

#include<bits/stdc++.h>

using namespace std;

struct Document {
    string title;
    string url;
    int wordcount;
    unordered_map<int, int> terms;
    Document();
    Document(string title, string url);
};

ostream& operator<<(ostream& stream, Document &d);
istream& operator>>(istream& stream, Document &d);

#endif //DOCUMENT_HPP
