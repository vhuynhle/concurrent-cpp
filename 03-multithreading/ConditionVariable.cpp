#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

bool dataReady { false };
std::mutex mutex_;
std::condition_variable condVar;

void doTheWork()
{
    std::cout << "Processing shared data.\n";
}

void waitingForWork()
{
    std::cout << "Worker: Waiting for work.\n";
    std::unique_lock lock { mutex_ };
    condVar.wait(lock, []() { return dataReady; });
    doTheWork();
    std::cout << "Work done.\n";
}

void setDataReady()
{
    {
        std::lock_guard<std::mutex> lock { mutex_ };
        dataReady = true;
    }
    std::cout << "Sender: Data is ready.\n";
    condVar.notify_one();
}

int main()
{
    std::cout << "\n";
    std::jthread t1 { waitingForWork };
    std::jthread t2 { setDataReady };

    return 0;
}
