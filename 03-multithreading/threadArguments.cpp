#include <chrono>
#include <iostream>
#include <thread>

class Sleeper {
public:
    explicit Sleeper(int& i_)
        : i { i_ }
    {
    }

    void operator()(int k)
    {
        for (unsigned j = 0; j < 5; ++j) {
            std::this_thread::sleep_for(std::chrono::milliseconds { 100 });
            i += k;
        }
        std::cout << std::this_thread::get_id() << "\n";
    }

private:
    int& i;
};

int main()
{
    std::cout << '\n';

    int valSleeper { 1000 };
    std::thread t { Sleeper { valSleeper }, 5 };

    // Warning: Lifetime issue
    t.detach();

    // Warning: data race
    std::cout << valSleeper << std::endl;

    std::cout << '\n';
}
