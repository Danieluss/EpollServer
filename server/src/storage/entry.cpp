#include"storage/entry.h"

ostream &operator<<(ostream &stream, Entry &e) {
    return (stream << e.title << "\n" << e.url << "\n" << e.description);
}