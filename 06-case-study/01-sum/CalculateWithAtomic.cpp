#include <atomic>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

constexpr std::size_t kSize { 100'000'000 };

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
    std::cout << "sum.is_lock_free? " << sum.is_lock_free() << "\n";
    for (auto i : randomValues) {
        sum += i;
    }

    const auto endTime { std::chrono::steady_clock::now() };
    const auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);

    std::cout << "Time for addition: " << duration.count() << " seconds.\n"
              << "Result: " << sum << "\n";

    std::atomic<std::int64_t> sum2 { 0 };
    const auto startTime2 { std::chrono::steady_clock::now() };
    for (auto i : randomValues) {
        sum2.fetch_add(i);
    }

    const auto endTime2 { std::chrono::steady_clock::now() };
    const auto duration2 = std::chrono::duration_cast<std::chrono::duration<double>>(endTime2 - startTime2);
    std::cout << "Time for addition: " << duration2.count() << " seconds.\n"
              << "Result: " << sum << "\n";

    return 0;
}
