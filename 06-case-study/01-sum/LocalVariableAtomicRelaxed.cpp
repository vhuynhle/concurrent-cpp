#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <numeric>
#include <random>
#include <thread>
#include <vector>

constexpr std::size_t kSize { 100'000'000 };
constexpr std::size_t kFir = 25000000;
constexpr std::size_t kSec = 50000000;
constexpr std::size_t kThi = 75000000;
constexpr std::size_t kFou = 100000000;

void sumUp(std::atomic<std::int64_t>& sum, std::vector<int>& val,
    std::size_t start,
    std::size_t end)
{
    const std::int64_t tmp = std::accumulate(val.begin() + start,
        val.begin() + end, std::int64_t { 0 });

    // The only guarantee we need is that the summations occur and are atomic
    // so memory_order_relaxed is OK.
    sum.fetch_add(tmp, std::memory_order_relaxed);
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
    std::atomic<std::int64_t> sum {};

    std::thread t1 { sumUp, std::ref(sum), std::ref(randomValues), 0, kFir };
    std::thread t2 { sumUp, std::ref(sum), std::ref(randomValues), kFir, kSec };
    std::thread t3 { sumUp, std::ref(sum), std::ref(randomValues), kSec, kThi };
    std::thread t4 { sumUp, std::ref(sum), std::ref(randomValues), kThi, kFou };

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    const auto endTime { std::chrono::steady_clock::now() };
    const auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);

    std::cout << "Time for addition: " << duration.count() << " seconds.\n"
              << "Result: " << sum << "\n";

    return 0;
}
