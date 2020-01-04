#include"storage/document.hpp"
#include"utils.h"
#include"utils.cpp"

Document::Document() {}

Document::Document(string title, string url) : title(title), url(url) {}

ostream& operator<<(ostream& stream, Document &d) {
    stream << d.title << "\n";
    stream << d.url << "\n";
    stream << d.wordcount << "\n";
    stream << d.terms << "\n";
    return stream;
}

istream& operator>>(istream& stream, Document &d) {
    getline(stream, d.title);
    stream >> d.url;
    stream >> d.wordcount;
    stream >> d.terms;
    return stream;
}