#include"storage/document.hpp"
#include"utils.h"
#include"utils.cpp"

Document::Document() {
    wordcount = 0;
}

Document::Document(string title, string url) : Document() {
    this->title = sanitizeTitle(title);
    this->url = url;
}

string Document::sanitizeTitle(string s) {
    for(int i=0; i < SIZE(s); i++) {
        if(s[i] == '\n' && (i > 0 || s[i-1] >= 0)) {
            s[i] = ' ';
        }
    }
    return s;
}

ostream& operator<<(ostream& stream, Document &d) {
    stream << d.title << "\n";
    stream << d.url << "\n";
    stream << d.wordcount << "\n";
    stream << d.terms << "\n";
    return stream;
}

istream& operator>>(istream& stream, Document &d) {
    string s;
    getline(stream, s);
    getline(stream, d.title);
    stream >> d.url;
    stream >> d.wordcount;
    stream >> d.terms;
    return stream;
}