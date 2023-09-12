#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

struct CriticalData {
    std::mutex mut;
};

void deadlock(CriticalData& a, CriticalData& b)
{
    a.mut.lock();
    std::cout << "Get the first mutex: " << (void*)&a << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    b.mut.lock();
    std::cout << "Get the second mutex" << (void*)&b << "\n";

    // Do work
    // ...
    // Work done

    a.mut.unlock();
    b.mut.unlock();
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
