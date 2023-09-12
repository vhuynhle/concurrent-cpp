
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

std::mutex coutMutex;

thread_local std::string s { "Hello from " };

void addThreadLocal(std::string const& s2)
{
    s += s2;

    // Protect std::cout
    std::lock_guard lock { coutMutex };
    std::cout << s << "\t&s: " << &s << "\n\n";
}

int main()
{
    std::jthread t1 { addThreadLocal, std::string { "t1" } };
    std::jthread t2 { addThreadLocal, std::string { "t2" } };
    std::jthread t3 { addThreadLocal, std::string { "t3" } };
    std::jthread t4 { addThreadLocal, std::string { "t4" } };
    return 0;
}
