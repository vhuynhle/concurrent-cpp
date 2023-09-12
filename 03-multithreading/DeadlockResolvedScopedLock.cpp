#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

struct CriticalData {
    std::mutex mut;
};

void deadlock(CriticalData& a, CriticalData& b)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    std::scoped_lock lock { a.mut, b.mut };
    std::cout << "Thread: " << std::this_thread::get_id() << " gets both mutexes\n";

    // Do work
    // ...
    // Work done
}

int main()
{
    CriticalData c1;
    CriticalData c2;

    std::thread t1 { [&]() { deadlock(c1, c2); } };
    std::thread t2 { [&]() { deadlock(c2, c1); } };

    t1.join();
    t2.join();
    return 0;
}
