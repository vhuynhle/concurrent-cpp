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
    const std::int64_t sum {
        std::accumulate(randomValues.cbegin(),
            randomValues.cend(), std::int64_t { 0 })
    };

    const auto endTime { std::chrono::steady_clock::now() };
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

    std::cout << "Time for addition: " << duration.count() << " ms\n"
              << "Result: " << sum << "\n";

    return 0;
}
