#include <iostream>
#include <thread>

int main()
{
    // No join, no detach -> std::terminate
    std::thread t { []() { std::cout << std::this_thread::get_id() << '\n'; } };
    t.join();

    return 0;
}
