#include <iostream>
#include <mutex>
#include <thread>

class Critical {
public:
    void interface1() const
    {
        std::lock_guard lock { mutex_ };
        implementation1();
    }

    void interface2() const
    {
        std::lock_guard lock { mutex_ };
        implementation1();
        implementation2();
        implementation3();
    }

private:
    mutable std::mutex mutex_;

    void implementation1() const
    {
        std::cout << "implementation1: " << std::this_thread::get_id() << '\n';
    }

    void implementation2() const
    {
        std::cout << "implementation2: " << std::this_thread::get_id() << '\n';
    }

    void implementation3() const
    {
        std::cout << "implementation3: " << std::this_thread::get_id() << '\n';
    }
};

int main()
{
    Critical c;

    std::thread t1 { [&c]() -> void {
        c.interface1();
    } };

    std::thread t2 { [&c]() -> void {
        c.interface2();
    } };

    std::thread t3 { [&c]() -> void {
        c.interface1();
        c.interface2();
    } };

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
