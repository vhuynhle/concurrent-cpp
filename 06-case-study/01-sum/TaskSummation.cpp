#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <future>
#include <iostream>
#include <numeric>
#include <random>
#include <thread>
#include <utility>
#include <vector>

constexpr std::size_t kSize { 100'000'000 };
constexpr std::size_t kFir = 25000000;
constexpr std::size_t kSec = 50000000;
constexpr std::size_t kThi = 75000000;
constexpr std::size_t kFou = 100000000;

void sumUp(std::promise<std::int64_t>&& prom,
    std::vector<int>& val, std::size_t start, std::size_t end)
{
    const std::int64_t s = std::accumulate(val.begin() + start,
        val.begin() + end, std::int64_t { 0 });
    prom.set_value(s);
}

int main()
{
    std::vector<int> randomValues;
    randomValues.reserve(kSize);

    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<int> dist(1, 10);

    std::cout << "Generating data ...\n";
    for (std::size_t i { 0 }; i < kSize; ++i) {
        randomValues.push_back(dist(engine));
    }
    std::cout << "Done generating data.\n";

    const auto startTime { std::chrono::steady_clock::now() };
    std::promise<std::int64_t> p1;
    std::promise<std::int64_t> p2;
    std::promise<std::int64_t> p3;
    std::promise<std::int64_t> p4;

    auto f1 = p1.get_future();
    auto f2 = p2.get_future();
    auto f3 = p3.get_future();
    auto f4 = p4.get_future();

    std::thread t1 { sumUp, std::move(p1), std::ref(randomValues), 0, kFir };
    std::thread t2 { sumUp, std::move(p2), std::ref(randomValues), kFir, kSec };
    std::thread t3 { sumUp, std::move(p3), std::ref(randomValues), kSec, kThi };
    std::thread t4 { sumUp, std::move(p4), std::ref(randomValues), kThi, kFou };

    const auto sum = f1.get() + f2.get() + f3.get() + f4.get();

    const auto endTime { std::chrono::steady_clock::now() };
    const auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);

    std::cout << "Time for addition: " << duration.count() << " seconds.\n"
              << "Result: " << sum << "\n";

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
