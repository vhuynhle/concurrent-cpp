#include <iostream>
#include <stop_token>
#include <thread>
#include <vector>
using namespace std::literals;

void func(std::stop_token stoken)
{
    int counter { 0 };

    auto const thread_id = std::this_thread::get_id();

    // callBack will be called when stoken is requested
    std::stop_callback callBack(stoken, [&counter, thread_id] {
        std::cout << "[callback] Thread id: " << thread_id << "; counter: " << counter << '\n';
    });

    while (counter < 10) {
        std::this_thread::sleep_for(0.2s);
        ++counter;
    }
}

int main()
{
    constexpr auto kNumThreads { 10 };
    std::vector<std::jthread> threads;
    threads.reserve(kNumThreads);

    for (int i { 0 }; i < kNumThreads; ++i) {
        threads.emplace_back(func);
    }

    std::this_thread::sleep_for(1s);

    for (auto& t : threads) {
        t.request_stop();
    }

    return 0;
}
