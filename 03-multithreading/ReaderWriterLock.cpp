#include <chrono>
#include <iostream>
#include <map>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <thread>

std::map<std::string, int> teleBook {
    { "Dijkstra", 1972 },
    { "Scott", 1976 },
    { "Ritchie", 1983 },
};

std::shared_timed_mutex teleBookMutex;

void addToTeleBook(std::string const& name, int number)
{
    std::lock_guard<std::shared_timed_mutex> writerLock { teleBookMutex };
    std::cout << "\nSTARTING UPDATE " << name;
    std::this_thread::sleep_for(std::chrono::milliseconds { 500 });
    teleBook[name] = number;
    std::cout << "... ENDING UPDATE " << name << '\n';
}

void printNumber(std::string const& name)
{
    std::shared_lock<std::shared_timed_mutex> readerLock { teleBookMutex };
    auto it = teleBook.find(name);
    if (it != teleBook.end()) {
        std::cout << name << ": " << it->second << '\n';
    } else {
        std::cout << name << ": UNKNOWN number\n";
    }
}

int main()
{
    std::thread reader1 { []() { printNumber("Scott"); } };
    std::thread reader2 { []() { printNumber("Ritchie"); } };
    std::thread reader3 { []() { printNumber("Dijkstra"); } };
    std::thread writer1 { []() { addToTeleBook("Bjarne", 1965); } };
    std::thread reader4 { []() { printNumber("Bill"); } };

    reader1.join();
    reader2.join();
    reader3.join();
    reader4.join();
    writer1.join();

    std::cout << "The new telephone book:\n";
    for (auto& [name, number] : teleBook) {
        std::cout << '\t' << name << ": " << number << '\n';
    }

    return 0;
}
