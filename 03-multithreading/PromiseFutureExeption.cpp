#include <exception>
#include <format>
#include <future>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <utility>

struct Div {
    void operator()(std::promise<int>&& intPromise, int a, int b)
    {
        if (b == 0) {
            const auto error_message = std::format("Division by zero: {}/{}", a, b);
            intPromise.set_exception(std::make_exception_ptr(std::runtime_error { error_message }));
        } else {
            intPromise.set_value(a / b);
        }
    }
};

void executeDivision(int nom, int denom)
{
    std::promise<int> divPromise;
    std::future<int> divResult = divPromise.get_future();

    Div div;
    std::thread divThread(div, std::move(divPromise), nom, denom);

    try {
        const auto result = divResult.get();
        std::cout << nom << " / " << denom << " = " << result << '\n';
    } catch (std::runtime_error const& e) {
        std::cout << e.what() << '\n';
    }

    divThread.join();
}

int main()
{
    executeDivision(20, 0);
    executeDivision(20, 10);
    return 0;
}
