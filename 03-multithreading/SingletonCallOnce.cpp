#include <iostream>
#include <mutex>

class Singleton {

public:
    static Singleton& getInstance()
    {
        std::call_once(oflag, [] { instance = new Singleton {}; });
        return *instance;
    }

    Singleton(Singleton const&) = delete;

private:
    static std::once_flag oflag;
    static Singleton* instance;

    Singleton()
    {
        std::cout << "Singleton constructed.\n";
    }
};

std::once_flag Singleton::oflag;
Singleton* Singleton::instance;

int main()
{
    auto& instance_1 = Singleton::getInstance();
    auto& instance_2 = Singleton::getInstance();

    if (&instance_1 != &instance_2) {
        std::cout << "NOT A SINGLETON!\n";
    } else {
        std::cout << "Ok\n";
    }

    return 0;
}
