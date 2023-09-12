#include <iostream>
#include <mutex>
#include <thread>
std::once_flag onceFlag;

void do_once()
{
    std::call_once(onceFlag, [] { std::cout << "Only once.\n"; });
}

void do_once2()
{
    std::call_once(
        onceFlag, [] { std::cout << "Only once2\n"; });
}

int main()
{
    std::jthread t1 { do_once };
    std::jthread t2 { do_once };
    std::jthread t3 { do_once };
    std::jthread t4 { do_once };
    std::jthread t5 { do_once2 };
    std::jthread t6 { do_once2 };
    return 0;
}
