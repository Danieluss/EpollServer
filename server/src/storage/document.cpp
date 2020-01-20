#include"storage/document.h"
#include"utils.h"

Document::Document() {
    wordcount = 0;
}

Document::Document(string title, string url, string description) : Document() {
    this->title = sanitizeTitle(title);
    this->url = url;
    this->description = sanitizeTitle(description);
}

string Document::sanitizeTitle(string s) {
    for (int i = 0; i < SIZE(s); i++) {
        if (s[i] == '\n' || s[i] == '\r') {
            s[i] = ' ';
        }
    }
    return s;
}

ostream &operator<<(ostream &stream, Document &d) {
    stream << d.title << "\n";
    stream << d.url << "\n";
    stream << d.description << "\n";
    stream << d.wordcount << "\n";
    stream << d.terms << "\n";
    return stream;
}

istream &operator>>(istream &stream, Document &d) {
    string s;
    getline(stream, s);
    getline(stream, d.title);
    stream >> d.url;
    getline(stream, s);
    getline(stream, d.description);
    stream >> d.wordcount;
    stream >> d.terms;
    return stream;
}