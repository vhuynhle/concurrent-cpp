#include <cstddef>
#include <format>
#include <iostream>
#include <latch>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

static constexpr std::size_t kTasks { 6 };

std::latch workDone { kTasks };
std::latch goHome { 1 };
std::mutex coutMutex;

void synchronizedOut(auto const& s)
{
    std::lock_guard guard { coutMutex };
    std::cout << s;
}

class Worker {
public:
    Worker(std::string n)
        : name { std::move(n) }
    {
    }

    void operator()()
    {
        // Notify the boss when work is done
        synchronizedOut(name + ": Work done!\n");
        workDone.count_down();

        // Waiting before going home
        goHome.wait();
        synchronizedOut(name + ": Goodbye!\n");
    }

private:
    std::string name;
};

int main()
{
    std::cout << "BOSS: START WORKING.\n";
    std::vector<std::thread> workers;
    workers.reserve(kTasks);
    for (std::size_t i { 0 }; i < kTasks; i++) {
        workers.emplace_back(Worker { std::format("Worker {}", i) });
    }

    workDone.wait();

    std::cout << "Boss: OK, YOU CAN GO HOME NOW.\n";
    goHome.count_down();

    for (auto& worker : workers) {
        worker.join();
    }

    return 0;
}
