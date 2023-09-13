#include <chrono>
#include <future>
#include <iostream>
#include <thread>

int main()
{
    const auto begin = std::chrono::steady_clock::now();

    auto asyncLazy = std::async(std::launch::deferred,
        [] { return std::chrono::steady_clock::now(); });

    auto asyncEager = std::async(std::launch::async,
        [] { return std::chrono::steady_clock::now(); });

    std::this_thread::sleep_for(std::chrono::seconds { 1 });

    const auto lazyStart = asyncLazy.get() - begin;
    const auto eagerStart = asyncEager.get() - begin;

    std::cout << "asyncLazy evaluated after "
              << std::chrono::duration_cast<std::chrono::seconds>(lazyStart).count()
              << " seconds.\n";
    std::cout << "asyncEager evaluated after "
              << std::chrono::duration_cast<std::chrono::seconds>(eagerStart).count()
              << " seconds.\n";

    return 0;
}
