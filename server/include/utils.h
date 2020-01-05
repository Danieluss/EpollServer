#ifndef SEARCHENGINE_UTILS_H
#define SEARCHENGINE_UTILS_H

#include<bits/stdc++.h>
#define SIZE(a) ((int)a.size())
using namespace std;

template<class T, class U>
ostream& operator<<(ostream& stream, pair<T, U> &p);

template<class T, class U>
istream& operator>>(istream& stream, pair<T, U> &p);

template<class T>
ostream& operator<<(ostream& stream, vector<T> &v);

template<class T>
istream& operator>>(istream& stream, vector<T> &v);

template<class T>
ostream& operator<<(ostream& stream, unordered_set<T> &s);

template<class T>
istream& operator>>(istream& stream, unordered_set<T> &s);

template<class T, class U>
ostream& operator<<(ostream& stream, unordered_map<T, U> &m);

template<class T, class U>
istream& operator>>(istream& stream, unordered_map<T, U> &m);

template<class T>
void readObjectFromFile(string filename, T &t);

template<class T>
void writeObjectToFile(string filename, T &t);

const string filesPath = "res/";

#endif //SEARCHENGINE_UTILS_H
