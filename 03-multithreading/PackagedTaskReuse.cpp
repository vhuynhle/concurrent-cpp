#include <format>
#include <functional>
#include <future>
#include <iostream>
#include <thread>
#include <utility>
#include <vector>

void calcProduct(std::packaged_task<int(int, int)>& task,
    std::vector<std::pair<int, int>> const& pairs)
{
    for (auto [first, second] : pairs) {
        auto f = task.get_future();
        task(first, second);
        std::cout << std::format("{} * {} = {}\n", first, second, f.get());
        task.reset();
    }
}

int main()
{
    std::vector<std::pair<int, int>> allPairs {
        { 1, 2 },
        { 2, 3 },
        { 3, 4 },
        { 4, 5 },
    };

    std::packaged_task<int(int, int)> task([](int a, int b) { return a * b; });
    calcProduct(task, allPairs);

    std::thread t(calcProduct, std::ref(task), allPairs);
    t.join();

    return 0;
}
