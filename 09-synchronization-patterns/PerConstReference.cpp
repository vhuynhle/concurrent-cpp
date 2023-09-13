#include <iostream>
#include <thread>

const bool* givenToFunction { nullptr };

void perConstReference(const bool& b)
{
    givenToFunction = &b;
    std::cout << __func__ << ": b = " << b << "; &b = " << (void*)&b << "\n";
}

int main()
{
    bool b { false };
    std::thread t { perConstReference, b };
    t.join();

    std::cout << __func__ << ": b = " << b << "; &b = " << (void*)&b << "\n";

    if (givenToFunction == &b) {
        std::cout << "Correctly passed by reference\n";
    } else {
        std::cout << "Passed by copy instead of by reference!\n";
    }

    return 0;
}
