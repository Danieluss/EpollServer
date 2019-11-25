#ifndef SEARCHENGINE_SYNCH_LOGGER_H
#define SEARCHENGINE_SYNCH_LOGGER_H

#include <mutex>
#include <iostream>
#include <thread>
#include <string>

class SynchLogger {
    mutable std::shared_mutex mutex;

    template<typename T>
    friend const SynchLogger &operator<<(const SynchLogger &logger, const T &value);
public:
    template<typename... Args>
    void print(const char *format, Args... args) const {
        std::unique_lock lock(mutex);
        printf(("[%d] - " + std::string(format)).c_str(), std::this_thread::get_id(), args...);
    }

    template<typename... Args>
    void eprint(const char *format, Args... args) const {
        std::unique_lock lock(mutex);
        fprintf(stderr, ("[%d] - " + std::string(format)).c_str(), std::this_thread::get_id(), args...);
    }

    template<typename... Args>
    void perror(const char *msg) const {
        std::unique_lock lock(mutex);
        std::cerr<<"["<<std::this_thread::get_id()<<"] - "<<msg<<std::endl;
        perror("");
    }
};

const SynchLogger synch_log;

template<typename T>
const SynchLogger &operator<<(const SynchLogger &logger, const T &value) {
    std::unique_lock lock(logger.mutex);
    std::cout<<value;
    return logger;
}

#endif //SEARCHENGINE_SYNCH_LOGGER_H
