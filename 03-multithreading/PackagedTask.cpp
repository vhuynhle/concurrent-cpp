
#include <cstdint>
#include <deque>
#include <future>
#include <iostream>
#include <numeric>
#include <thread>
#include <utility>
#include <vector>

class SumUp {
public:
    std::int32_t operator()(std::int32_t begin, std::int32_t end)
    {
        std::int64_t sum { 0 };
        for (std::int32_t i { begin }; i < end; ++i) {
            sum += i;
        }
        return sum;
    }
};

int main()
{
    // Wrap the tasks and get the futures
    std::deque<std::packaged_task<std::int32_t(std::int32_t, std::int32_t)>> allTasks;
    std::vector<std::future<std::int32_t>> fs;
    for (std::int32_t i { 0 }; i < 4; ++i) {
        allTasks.emplace_back(SumUp {});
        fs.push_back(allTasks.back().get_future());
    }

    std::int32_t begin { 1 };
    std::int32_t increment { 2500 };
    std::int32_t end { begin + increment };

    // Perform the calculation in separate threads
    while (!allTasks.empty()) {
        auto myTask = std::move(allTasks.front());
        allTasks.pop_front();
        std::thread sumThread { std::move(myTask), begin, end };

        begin = end;
        end += increment;
        sumThread.detach();
    }

    // Pickup the result
    const auto sum = std::accumulate(fs.begin(), fs.end(), 0LL, [](auto acc, auto& f) {
        return acc + f.get();
    });

    std::cout << "Sum [1.." << begin - 1 << "] = " << sum << std::endl;

    return 0;
}
