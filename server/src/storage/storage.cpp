#include"storage/storage.h"
#include"utils.h"

void Storage::load() {
    readObjectFromFile(termsFile, terms);
    readObjectFromFile(documentsFile, documents);
    nextTerm = SIZE(terms);
    nextDocument = SIZE(documents);
    // cerr << nextTerm << " " << nextDocument << "\n";
}

void Storage::save() {
    writeObjectToFile(termsFile, terms);
    writeObjectToFile(documentsFile, documents);
}

Storage::Storage() {
    load();
}

string Storage::tolower(string &s) {
    u16string w = convert.from_bytes(s);
    for (auto it = w.begin(); it != w.end(); it++) {
        (*it) = towlower(*it);
    }
    string res = convert.to_bytes(w);
    return res;
}

void Storage::add(HTMLParser &html) {
    Document doc(html.getTitle(), html.getUrl(), html.getDescription());
    documents[nextDocument] = doc;
    Document &d = documents[nextDocument];

    vector <string> words = html.getWords();
    for (string &w : words) {
        string u = tolower(w);
        if (!terms.count(u)) {
            Term t;
            t.id = nextTerm++;
            terms[u] = t;
        }
        Term &currentTerm = terms[u];
        currentTerm.documents.insert(nextDocument);
        if (!d.terms.count(currentTerm.id)) {
            d.terms[currentTerm.id] = 0;
        }
        d.terms[currentTerm.id]++;
        d.wordcount++;
    }
    nextDocument++;
}

vector<double> Storage::normalize(vector<double> v) {
    double l = 0;
    for (double vi : v) {
        l += vi * vi;
    }
    l = sqrt(l);
    for (double &vi : v) {
        vi /= l;
    }
    return v;
}

double Storage::dot(vector<double> a, vector<double> b) {
    double res = 0;
    for (int i = 0; i < SIZE(a); i++) {
        res += a[i] * b[i];
    }
    return res;
}

double Storage::cosv(vector<double> a, vector<double> b) {
    return dot(normalize(a), normalize(b));
}

double Storage::computeScore(Document &document, vector <pii> &activeTerms) {
    int n = SIZE(activeTerms);
    vector<double> d(n), q(n);
    for (int i = 0; i < n; i++) {
        pii x = activeTerms[i];
        int termId = x.first;
        int df = x.second;
        q[i] = log(double(SIZE(documents)) / double(df));
        auto it = document.terms.find(termId);
        if (it == document.terms.end()) {
            continue;
        }
        int tf = it->second;
        d[i] = log(1 + tf);
    }
    return dot(d, q);
}

Entry Storage::prepareEntry(Document &d, vector <pii> &activeTerms) {
    Entry e;
    e.title = d.title;
    e.url = d.url;
    e.description = d.description;
    return e;
}

pair<vector<Entry>, int> Storage::query(int page, int pageSize, vector <string> q) {
    int limit = (page + 1) * pageSize;
    vector <pii> activeTerms;
    Term *minTerm = NULL;
    for (string s : q) {
        s = tolower(s);
        auto it = terms.find(s);
        if (it == terms.end()) {
            continue;
        }
        Term &t = it->second;
        activeTerms.push_back({t.id, SIZE(t.documents)});
        if (minTerm == NULL || SIZE(t.documents) < SIZE(minTerm->documents)) {
            minTerm = &t;
        }
    }

    if (minTerm == NULL) {
        return {{}, 0};
    }

    priority_queue <pair<double, int>> entries;

    for (int documentId : (minTerm->documents)) {
        double score = computeScore(documents[documentId], activeTerms);
        entries.push({-score, documentId});
        if (SIZE(entries) > limit) {
            entries.pop();
        }
    }

    vector <Entry> list;
    while (page * pageSize < SIZE(entries)) {
        auto p = entries.top();
        // cerr << -p.first << "\n";
        entries.pop();
        list.push_back(prepareEntry(documents[p.second], activeTerms));
    }
    reverse(list.begin(), list.end());
    return {list, (SIZE(minTerm->documents) - 1) / pageSize + 1};
}