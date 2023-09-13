#include <bits/chrono.h>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <future>
#include <iostream>
#include <mutex>
#include <numeric>
#include <random>
#include <thread>
#include <vector>

constexpr std::size_t kNum { 100'000'000 };

std::int64_t getDotProduct(std::vector<std::int32_t> const& v,
    std::vector<std::int32_t> const& w)
{
    const auto vSize = v.size();
    std::mutex coutMutex;
    auto f1 = std::async(std::launch::async,
        [&] {
            {
                std::lock_guard guard { coutMutex };
                std::cout << "async thread " << std::this_thread::get_id() << std::endl;
            }
            return std::inner_product(v.begin(), v.begin() + vSize / 4, w.begin(), 0LL);
        });
    auto f2 = std::async(std::launch::async,
        [&] {
            {
                std::lock_guard guard { coutMutex };
                std::cout << "async thread " << std::this_thread::get_id() << std::endl;
            }
            return std::inner_product(v.begin() + vSize / 4, v.begin() + vSize / 2,
                w.begin() + vSize / 4, 0LL);
        });
    auto f3 = std::async(std::launch::async,
        [&] {
            {
                std::lock_guard guard { coutMutex };
                std::cout << "async thread " << std::this_thread::get_id() << std::endl;
            }
            return std::inner_product(v.begin() + vSize / 2, v.begin() + 3 * vSize / 4,
                w.begin() + vSize / 2, 0LL);
        });
    auto f4 = std::async(std::launch::async,
        [&] {
            {
                std::lock_guard guard { coutMutex };
                std::cout << "async thread " << std::this_thread::get_id() << std::endl;
            }
            return std::inner_product(v.begin() + 3 * vSize / 4, v.end(),
                w.begin() + 3 * vSize / 4, 0LL);
        });

    return f1.get() + f2.get() + f3.get() + f4.get();
}

int main()
{
    const auto start = std::chrono::steady_clock::now();

    std::random_device rd;
    std::mt19937 engine { rd() };
    std::uniform_int_distribution<std::int32_t> dist { 0, 100 };

    std::vector<std::int32_t> v;
    std::vector<std::int32_t> w;

    v.reserve(kNum);
    w.reserve(kNum);

    for (std::size_t i { 0 }; i < kNum; ++i) {
        v.push_back(dist(engine));
        w.push_back(dist(engine));
    }

    const auto end = std::chrono::steady_clock::now();
    const auto duration_s = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Dot product: " << getDotProduct(v, w) << ", time: " << duration_s.count() << "s\n";

    return 0;
}
