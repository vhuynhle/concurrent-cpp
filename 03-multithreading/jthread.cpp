#include <ios>
#include <iostream>
#include <thread>

int main()
{
    std::cout << '\n'
              << std::boolalpha;

    std::jthread t { [] { std::cout << "Joinable std::jthread\n"; } };

    std::cout << "t.joinable() = " << t.joinable() << std::endl;

    return 0;
}
