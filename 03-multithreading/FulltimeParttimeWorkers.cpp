#include <barrier>
#include <cstddef>
#include <format>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

constexpr std::size_t kFulltimeWorkers { 5 };
constexpr std::size_t kParttimeWorkers { 4 };
constexpr std::size_t kTotalWorkers { kFulltimeWorkers + kParttimeWorkers };

std::barrier workDone { kTotalWorkers };
std::mutex coutMutex;

void synchronizedOut(auto const& s)
{
    std::lock_guard guard { coutMutex };
    std::cout << s;
}

class FulltimeWorker {
public:
    explicit FulltimeWorker(std::size_t i)
        : id { i }
    {
    }

    void operator()()
    {
        synchronizedOut(std::format("Fulltime worker {}: Morning work done!\n", id));
        workDone.arrive_and_wait();
        synchronizedOut(std::format("Fulltime worker {}: Afternoon work done!\n", id));
        workDone.arrive_and_wait();
    }

private:
    std::size_t id;
};

class ParttimeWorker {
public:
    explicit ParttimeWorker(std::size_t i)
        : id { i }
    {
    }

    void operator()()
    {
        synchronizedOut(std::format("Parttime worker {}: Morning work done!\n", id));
        workDone.arrive_and_drop(); // Wait until morning work is done
    }

private:
    std::size_t id;
};

int main()
{
    std::vector<std::thread> workers;
    workers.reserve(kTotalWorkers);

    for (std::size_t i { 0 }; i < kFulltimeWorkers; ++i) {
        workers.emplace_back(FulltimeWorker { i });
    }

    for (std::size_t i { 0 }; i < kParttimeWorkers; ++i) {
        workers.emplace_back(ParttimeWorker { i });
    }

    for (auto& w : workers) {
        w.join();
    }

    return 0;
}
