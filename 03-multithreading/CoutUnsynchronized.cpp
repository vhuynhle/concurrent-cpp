#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

class Worker {
public:
    Worker(std::string n)
        : name { std::move(n) }
    {
    }

    void operator()() const
    {
        for (int i { 0 }; i < 3; ++i) {
            // Start work
            std::this_thread::sleep_for(std::chrono::milliseconds { 200 });

            // End work
            std::cout << name << ": "
                      << "Work " << i + 1 << " done.\n";
        }
    }

private:
    std::string name;
};

int main()
{
    std::cout << "Boss: Let's start working.\n\n";

    std::vector<std::thread> threads {};
    threads.emplace_back(Worker { "Herb" });
    threads.emplace_back(Worker { "Andrei" });
    threads.emplace_back(Worker { "Scott" });
    threads.emplace_back(Worker { "Bjarne" });

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "\nBoss: Let's go home.\n\n";

    return 0;
}
