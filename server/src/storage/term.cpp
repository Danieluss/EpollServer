#include"storage/term.h"

istream &operator>>(istream &stream, Term &t) {
    stream >> t.id;
    int n;
    stream >> n;
    for (int i = 0; i < n; i++) {
        int a;
        stream >> a;
        t.documents.insert(a);
    }
    return stream;
}

ostream &operator<<(ostream &stream, Term &t) {
    stream << t.id << " ";
    stream << t.documents.size() << " ";
    for (int d : t.documents) {
        stream << d << " ";
    }
    return stream;
}