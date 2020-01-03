#ifndef STORAGE_HPP
#define STORAGE_HPP

#include<vector>
#include"crawler/html_parser.hpp"
#include"storage/entry.hpp"
#include"storage/term.hpp"
#include"storage/document.hpp"

using namespace std;

struct Storage {
private:
    int nextDocument;
    unordered_map<string, Term> terms;
    unordered_map<int, Document> documents;
public:
    Storage();
    void add(HTMLParser html);
    void save();
    vector<Entry> query(string query);
};

#endif //STORAGE_HPP
