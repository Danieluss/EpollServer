#ifndef STORAGE_HPP
#define STORAGE_HPP

#include<vector>
#include"crawler/html_parser.hpp"
#include"storage/entry.hpp"
#include"storage/term.hpp"
#include"storage/document.hpp"

using namespace std;
typedef pair<int, int> pii;

struct Storage {
private:
    int nextTerm;
    int nextDocument;
    unordered_map<string, Term> terms;
    unordered_map<int, Document> documents;
    string termsFile = "terms";
    string documentsFile = "documents";
    wstring_convert<codecvt_utf8_utf16<char16_t>,char16_t> convert;
    void load();
    vector<string> split(string &s);
    vector<double> normalize(vector<double> v);
    double cosv(vector<double> a, vector<double> b);
    double dot(vector<double> a, vector<double> b);
    double computeScore(Document &d, vector<pii> &activeTerms);
    Entry prepareEntry(Document &d, vector<pii> &activeTerms);
public:
    Storage();
    void add(HTMLParser &html);
    void save();
    
    pair<vector<Entry>, int> query(int page, int pageSize, string queryString);
    string tolower(string &s);
};

#endif //STORAGE_HPP
