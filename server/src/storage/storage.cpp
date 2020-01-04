#include"storage/storage.hpp"
#include"utils.h"
#include"utils.cpp"

void Storage::load() {
    readObjectFromFile(termsFile, terms);
    readObjectFromFile(documentsFile, documents);
    nextTerm = SIZE(terms);
    nextDocument = SIZE(documents);
}

void Storage::save() {
    writeObjectToFile(termsFile, terms);
    writeObjectToFile(documentsFile, documents);
}

Storage::Storage() {
    // load();
}

string Storage::tolower(string &s) {
    // cerr << setlocale(LC_CTYPE, "en_US.UTF-8") << "\n";
    wstring w(s.begin(), s.end());
    int cou=0;
    for(auto it = w.begin(); it != w.end(); it++) {
        cerr << (*it) << " ";
        *it = towlower(*it);
        cou++;
    }

    cerr << "Sizes comparison: " << cou << " " << s.size() << "\n";
    return string(w.begin(), w.end());
}

void Storage::add(HTMLParser html) {
    Document d(html.getTitle(), html.getUrl());
    documents[nextDocument] = d;

    vector<string> words = html.getWords();
    for(string &w : words) {
        string u = tolower(w);
        if(!terms.count(u)) {
            Term t;
            t.id = nextTerm++;
            terms[u] = t;
        }
        terms[u].documents.insert(nextDocument);
    }
    nextDocument++;
}

vector<string> Storage::split(string &s) {
    istringstream iss(s);
    return vector<string> (std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());
}

vector<double> normalize(vector<double> v) {
    double l = 0;
    for(double vi : v) {
        l+=vi*vi;
    }
    l = sqrt(l);
    for(double &vi : v) {
        vi/=l;
    }
    return v;
}

double dot(vector<double> a, vector<double> b) {
    a = normalize(a);
    b = normalize(b);
    double res=0;
    for(int i=0; i < SIZE(a); i++) {
        res+=a[i]*b[i];
    }
    return res;
}

double Storage::computeScore(Document &document, vector<pii> &activeTerms) {
    double score=0;
    int n = SIZE(activeTerms);
    vector<double> d(n), q(n);
    for(int i=0; i < n; i++) {
        pii x = activeTerms[i];
        int termId = x.first;
        int df = x.second;
        q[i] = log(double(SIZE(documents))/double(df));
        auto it = document.terms.find(termId);
        if(it == document.terms.end()) {
            continue;
        }
        int tf = it->second;
        d[i] = log(1+tf);
    }
    return score;
}

Entry Storage::prepareEntry(Document &d, vector<pii> &activeTerms) {
    Entry e;
    e.title = d.title;
    e.url = d.url;
    //TODO description
    return e;
}

vector<Entry> Storage::query(string text, int limit) {
    vector<string> q = split(text);
    vector<pii> activeTerms;
    Term *minTerm = NULL;
    for(string s : q) {
        s = tolower(s);
        auto it = terms.find(s);
        if(it == terms.end()) {
            continue;
        }
        Term &t = it->second;
        activeTerms.push_back({t.id, SIZE(t.documents)});
        if(minTerm == NULL || SIZE(t.documents) < SIZE(minTerm->documents)) {
            minTerm = &t;
        }
    }

    priority_queue<pair<double, int>> entries;

    for(int documentId : (minTerm->documents)) {
        double score = computeScore(documents[documentId], activeTerms);
        entries.push({-score, documentId});
        if(SIZE(entries) > limit) {
            entries.pop();
        }
    }

    vector<Entry> list;
    while(!entries.empty()) {
        auto p = entries.top();
        entries.pop();
        list.push_back(prepareEntry(documents[p.second], activeTerms));
    }
    reverse(list.begin(), list.end());
    return list;
}