#include"storage/entry.hpp"

ostream& operator<<(ostream& stream, Entry &e) {
    return (stream << e.title << "\n" << e.url << "\n" << e.description);
}