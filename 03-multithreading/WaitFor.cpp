#include <future>
#include <iostream>
#include <thread>
#include <utility>

using namespace std::literals::chrono_literals;

void getAnswer(std::promise<int>&& intPromise)
{
    std::this_thread::sleep_for(3s);
    intPromise.set_value(42);
}

int main()
{
    std::promise<int> answerPromise;
    auto fut = answerPromise.get_future();
    std::thread prodThread(getAnswer, std::move(answerPromise));

    std::future_status status {};
    do {
        status = fut.wait_for(0.2s);
        std::cout << "...doing something else while waiting ...\n";
    } while (status != std::future_status::ready);

    prodThread.join();

    return 0;
}
