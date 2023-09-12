#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

struct CriticalData {
    std::mutex mut;
};

void deadlock(CriticalData& a, CriticalData& b)
{
    std::unique_lock guard1 { a.mut, std::defer_lock };
    std::cout << "Thread: " << std::this_thread::get_id() << " first mutex (deferred lock)\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    std::unique_lock guard2 { b.mut, std::defer_lock };
    std::cout << "Thread: " << std::this_thread::get_id() << " second mutex (deferred lock)\n";

    std::lock(guard1, guard2);
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
