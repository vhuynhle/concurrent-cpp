#include <iostream>
#include <string>
#include <thread>

void executeTwoThreads()
{
    const std::string localString { "Local string" };

    std::thread t1 { [localString] {
        std::cout << "Per copy: " << localString << " @ " << &localString << '\n';
    } };

    std::thread t2 { [&localString] {
        // ERROR: localString may have been destructed by now
        // The program may print garbage characters.
        std::cout << "Per reference: " << localString << " @ " << &localString << '\n';
    } };

    t1.detach();
    t2.detach();
}

using namespace std::chrono_literals;

int main()
{
    executeTwoThreads();
    std::this_thread::sleep_for(1s);
    return 0;
}
