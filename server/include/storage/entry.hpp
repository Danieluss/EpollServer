#ifndef ENTRY_HPP
#define ENTRY_HPP

#include<bits/stdc++.h>

using namespace std;

struct Entry {
    string title;
    string url;
    string description;
};

ostream& operator<<(ostream& stream, Entry &e);

#endif //ENTRY_HPP
