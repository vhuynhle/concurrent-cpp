#include <future>
#include <iostream>
#include <stop_token>
#include <string>
#include <thread>
using namespace std::literals;

void func1(std::stop_token stopToken, std::string const& str)
{
    // Wait 1 second to ensure that callBack is
    // registered *after* stopSource.request_stop()
    std::this_thread::sleep_for(1s);
    if (stopToken.stop_requested()) {
        std::cout << str << ": stop requested\n";
    }
}

void func2(std::promise<void> prom,
    std::stop_token stopToken, std::string const& str)
{
    // Wait 1 second to ensure that callBack is
    // registered *after* stopSource.request_stop()
    std::this_thread::sleep_for(1s);
    std::stop_callback callBack {
        stopToken,
        [&str]() {
            std::cout << str << ": Stop requested\n";
        }
    };
    prom.set_value();
}

int main()
{
    std::stop_source stopSource;
    std::stop_token stopToken { stopSource.get_token() };

    std::thread t1 { func1, stopToken, "std::thread" };
    std::jthread t2 { func1, stopToken, "std::jthread" };

    auto fut1 = std::async([stopToken] {
        std::this_thread::sleep_for(1s);
        if (stopToken.stop_requested()) {
            std::cout << "std::async: Stop requested\n";
        }
    });

    std::promise<void> prom;
    auto fut2 = prom.get_future();
    std::thread t3 { func2, std::move(prom), stopToken, "std::promise" };

    // This will request stop from *all* stop token created from
    // this source
    stopSource.request_stop();

    if (stopToken.stop_requested()) {
        std::cout << "main: Stop requested\n";
    }

    t1.join();
    t3.join();

    return 0;
}
