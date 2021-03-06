#ifndef SEARCHENGINE_UTILS_H
#define SEARCHENGINE_UTILS_H

#include<bits/stdc++.h>
#include "definitions.h"

#define SIZE(a) ((int)a.size())
using namespace std;

template<class T, class U>
ostream &operator<<(ostream &stream, pair <T, U> &p) {
    return (stream << p.first << "\n" << p.second);
}

template<class T, class U>
istream &operator>>(istream &stream, pair <T, U> &p) {
    return (stream >> p.first >> p.second);
}

template<class T>
ostream &operator<<(ostream &stream, vector <T> &v) {
    stream << SIZE(v) << "\n";
    for (T &t : v) {
        stream << t << "\n";
    }
    return stream;
}

template<class T>
istream &operator>>(istream &stream, vector <T> &v) {
    v.clear();
    int n;
    stream >> n;
    v.resize(n);
    for (T &t : v) {
        stream >> t;
    }
    return stream;
}

template<class T>
ostream &operator<<(ostream &stream, unordered_set <T> &s) {
    vector <T> v;
    for (T t : s) {
        v.push_back(t);
    }
    return (stream << v);
}

template<class T>
istream &operator>>(istream &stream, unordered_set <T> &s) {
    s.clear();
    vector <T> v;
    stream >> v;
    for (T &t : v) {
        s.insert(t);
    }
    return stream;
}

template<class T, class U>
ostream &operator<<(ostream &stream, unordered_map <T, U> &m) {
    vector <pair<T, U>> v;
    for (pair <T, U> t : m) {
        v.push_back(t);
    }
    return (stream << v);
}

template<class T, class U>
istream &operator>>(istream &stream, unordered_map <T, U> &m) {
    m.clear();
    vector <pair<T, U>> v;
    stream >> v;
    for (pair <T, U> &t : v) {
        m[t.first] = t.second;
    }
    return stream;
}

template<class T>
void readObjectFromFile(string filename, T &t) {
    ifstream stream(filesPath + filename);
    if (stream.fail()) {
        return;
    }
    stream >> t;
    stream.close();
}

template<class T>
void writeObjectToFile(string filename, T &t) {
    filename = filesPath + filename;
    string currentFilename = filename + to_string(time(0));
    ofstream stream(currentFilename);
    stream << t;
    stream.close();
    if (rename(currentFilename.c_str(), filename.c_str())) {
        throw runtime_error(strerror(errno));
    }
}

vector<string> str_split(string s);

#endif //SEARCHENGINE_UTILS_H
