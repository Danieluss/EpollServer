#ifndef SEARCHENGINE_SYNCH_COUNTER_H
#define SEARCHENGINE_SYNCH_COUNTER_H

#include <mutex>
#include <shared_mutex>

class SynchCounter {
    volatile int value = 0;
    mutable std::shared_mutex mutex;

public:

    SynchCounter& zero() {
        std::unique_lock lock(mutex);
        value = 0;
        return *this;
    }

    SynchCounter &operator++() {
        std::unique_lock lock(mutex);
        value++;
        return *this;
    }

    SynchCounter &operator--() {
        std::unique_lock lock(mutex);
        value--;
        return *this;
    }

    int get() const {
        std::shared_lock lock(mutex);
        return value;
    }

    operator int() const { return get(); }

};

#endif //SEARCHENGINE_SYNCH_COUNTER_H
