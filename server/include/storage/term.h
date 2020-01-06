#ifndef TERM_H
#define TERM_H

#include<bits/stdc++.h>

using namespace std;

struct Term {
    int id;
    unordered_set<int> documents;
};

istream &operator>>(istream &stream, Term &t);

ostream &operator<<(ostream &stream, Term &t);

#endif //TERM_H
