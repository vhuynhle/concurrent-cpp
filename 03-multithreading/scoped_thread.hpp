#pragma once

#include <stdexcept>
#include <thread>

class scoped_thread {
public:
    explicit scoped_thread(std::thread t_)
        : t { std::move(t_) }
    {
        if (!t.joinable()) {
            throw std::logic_error { "No thread" };
        }
    }

    ~scoped_thread()
    {
        t.join();
    }

    scoped_thread(scoped_thread const&) = delete;
    scoped_thread(scoped_thread const&&) = delete;
    scoped_thread& operator=(scoped_thread const&) = delete;
    scoped_thread& operator=(scoped_thread&&) = delete;

private:
    std::thread t;
};
