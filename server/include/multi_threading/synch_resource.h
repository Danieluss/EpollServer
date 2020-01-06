#ifndef SEARCHENGINE_RESOURCE_H
#define SEARCHENGINE_RESOURCE_H

#include <mutex>
#include <shared_mutex>
#include <utility>

template<typename T>
class SynchResource {
    volatile T value;
    mutable std::shared_mutex mutex;

public:

    SynchResource(T value) : value(value) {
    }

    void write(T value) {
        std::unique_lock lock(mutex);
        this->value = value;
    }

    T read() {
        std::shared_lock lock(mutex);
        return value;
    }

    operator T() { return read(); }

    operator T() const { return read(); }

};


#endif //SEARCHENGINE_RESOURCE_H
