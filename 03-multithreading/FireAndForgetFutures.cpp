#include <chrono>
#include <future>
#include <iostream>
#include <thread>

int main()
{
    (void)std::async(std::launch::async, []() {
        std::cout << "First thread\n";
        std::this_thread::sleep_for(std::chrono::seconds { 1 });
    });

    (void)std::async(std::launch::async, []() {
        std::cout << "Second thread\n";
        std::this_thread::sleep_for(std::chrono::seconds { 1 });
    });

    std::cout << "Main thread\n";
    return 0;
}
