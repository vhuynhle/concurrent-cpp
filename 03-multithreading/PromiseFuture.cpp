#include <future>
#include <iostream>
#include <thread>
#include <utility>

void product(std::promise<int>&& intPromise, int a, int b)
{
    intPromise.set_value(a * b);
}

struct Div {
    void operator()(std::promise<int>&& intPromise, int a, int b) const
    {
        intPromise.set_value(a / b);
    }
};

/*
   Promise: Sender                        Future: Receiver
        |                                        |
        | [SET]    ___________________           | [GET]
        |-------->| Channel           |--------->
                   -------------------
 */
int main()
{
    int a = 20;
    int b = 10;

    std::promise<int> prodPromise;
    std::promise<int> divPromise;

    std::future<int> prodResult = prodPromise.get_future();
    std::future<int> divResult = divPromise.get_future();

    std::thread prodThread(product, std::move(prodPromise), a, b);
    std::thread divThread(Div {}, std::move(divPromise), a, b);

    std::cout << "20 * 10 = " << prodResult.get() << '\n';
    std::cout << "20 / 10 = " << divResult.get() << '\n';

    prodThread.join();
    divThread.join();

    return 0;
}
