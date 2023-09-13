#include <chrono>
#include <cstddef>
#include <format>
#include <iostream>
#include <latch>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

constexpr std::size_t kTasks { 6U };
std::latch workDone { kTasks };
std::mutex coutMutex;

void synchronizedOut(auto const& s)
{
    std::lock_guard guard { coutMutex };
    std::cout << s;
}

class Worker {
public:
    explicit Worker(std::size_t id_)
        : id { id_ }
    {
    }

    void operator()() const
    {
        std::this_thread::sleep_for(std::chrono::milliseconds { 500 * id });
        synchronizedOut(std::format("Worker {}: Work done!\n", id));
        workDone.arrive_and_wait();
        synchronizedOut(std::format("Worker {}: See you tomorrow!\n", id));
    }

private:
    std::size_t id;
};

int main()
{
    std::vector<std::thread> workers;
    workers.reserve(kTasks);

    for (std::size_t i { 0 }; i < kTasks; ++i) {
        workers.emplace_back(Worker(i));
    }

    for (auto& worker : workers) {
        worker.join();
    }

    return 0;
}
