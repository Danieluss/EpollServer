#ifndef LINK_H
#define LINK_H

#include<string>

using namespace std;

struct Link {
private:
    int depth;

    int compute_depth();

public:
    string url;

    Link(string url);

    friend bool operator<(const Link &l1, const Link &l2);
};

bool operator<(const Link &l1, const Link &l2);

#endif //LINK_H
