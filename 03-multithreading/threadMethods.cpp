#include <ios>
#include <iostream>
#include <thread>
#include <utility>

int main()
{
    using std::swap;
    std::cout << std::boolalpha;

    std::cout << "hardware_concurrency: " << std::thread::hardware_concurrency() << std::endl;
    ;
    std::thread t1 { [] { std::cout << "thread t1: id = " << std::this_thread::get_id() << '\n'; } };
    std::thread t2 { [] { std::cout << "thread t2: id = " << std::this_thread::get_id() << '\n'; } };

    std::cout << '\n';

    std::cout << "From main: t1 id = " << t1.get_id() << '\n';
    std::cout << "From main: t2 id = " << t2.get_id() << '\n';

    std::cout << "swap\n";
    swap(t1, t2);

    std::cout << "From main: t1 id = " << t1.get_id() << '\n';
    std::cout << "From main: t2 id = " << t2.get_id() << '\n';

    std::cout << '\n';
    std::cout << "t1 joinable? " << t1.joinable() << '\n';
    std::cout << "t2 joinable? " << t2.joinable() << '\n';

    t1.join();
    t2.join();

    std::cout << "t1 joinable? " << t1.joinable() << '\n';
    std::cout << "t2 joinable? " << t2.joinable() << '\n';

    return 0;
}
