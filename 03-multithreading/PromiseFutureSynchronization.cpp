#include <chrono>
#include <future>
#include <iostream>
#include <thread>
#include <utility>

void doTheWork()
{
    std::cout << "Processing shared data.\n";
}

void waitingForWork(std::future<void>&& fut)
{
    std::cout << "Worker: waiting for work.\n";
    fut.wait();
    doTheWork();
    std::cout << "Work done.\n";
}

void setDataReady(std::promise<void>&& prom)
{
    std::cout << "Sender: Producing data ...\n";
    std::this_thread::sleep_for(std::chrono::seconds { 1 });
    std::cout << "Sender: Data is ready.\n";
    prom.set_value();
}

int main()
{
    std::promise<void> sendReady;
    auto fut = sendReady.get_future();

    std::thread t1 { waitingForWork, std::move(fut) };
    std::thread t2 { setDataReady, std::move(sendReady) };

    t1.join();
    t2.join();

    return 0;
}
