#include <functional>
#include <ios>
#include <iostream>
#include <thread>
using namespace std::chrono_literals;

void byCopy(bool b)
{
    std::this_thread::sleep_for(1ms);
    std::cout << __func__ << ": " << b << '\n';
}

void byReference(bool& b)
{
    std::this_thread::sleep_for(1ms);
    std::cout << __func__ << ": " << b << '\n';
}

void byConstReference(const bool& b)
{
    std::this_thread::sleep_for(1ms);
    std::cout << __func__ << ": " << b << '\n';
}

int main()
{
    std::cout << std::boolalpha;
    bool shared { false };

    std::thread t1 { byCopy, shared };
    std::thread t2 { byReference, std::ref(shared) };
    std::thread t3 { byConstReference, std::cref(shared) };

    shared = true;
    t1.join();
    t2.join();
    t3.join();
}
