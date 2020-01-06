#ifndef ENTRY_H
#define ENTRY_H

#include<bits/stdc++.h>

using namespace std;

struct Entry {
    string title;
    string url;
    string description;
};

ostream &operator<<(ostream &stream, Entry &e);

#endif //ENTRY_H
