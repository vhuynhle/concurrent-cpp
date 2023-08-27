#include <iostream>
#include <thread>

void helloFunction()
{
    std::cout << "Hello from a function." << std::endl;
}

class helloFunctionObject {
public:
    void operator()() const
    {
        std::cout << "Hello from a function object." << std::endl;
    }
};

int main()
{
    std::thread t1 { helloFunction };
    std::thread t2 { helloFunctionObject {} };
    std::thread t3 { []() {
        std::cout << "Hello from a lambda" << std::endl;
    } };

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
