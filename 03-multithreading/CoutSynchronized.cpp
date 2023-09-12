#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

class Worker {
public:
    Worker(std::string n,
        std::mutex& m)
        : name { std::move(n) }
        , coutMutex { m }
    {
    }

    void operator()() const
    {
        for (int i { 0 }; i < 3; ++i) {
            // Start work
            std::this_thread::sleep_for(std::chrono::milliseconds { 200 });

            // End work

            // Access shared resource (cout)
            std::lock_guard lock { coutMutex };
            std::cout << name << ": "
                      << "Work " << i + 1 << " done.\n";
        }
    }

private:
    std::string name;
    std::mutex& coutMutex;
};

int main()
{
    std::cout << "Boss: Let's start working.\n\n";

    std::vector<std::thread> threads {};
    std::mutex coutMutex;
    threads.emplace_back(Worker { "Herb", coutMutex });
    threads.emplace_back(Worker { "Andrei", coutMutex });
    threads.emplace_back(Worker { "Scott", coutMutex });
    threads.emplace_back(Worker { "Bjarne", coutMutex });

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "\nBoss: Let's go home.\n\n";

    return 0;
}
