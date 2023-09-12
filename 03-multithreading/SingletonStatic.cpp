#include <iostream>

class Singleton {

public:
    static Singleton& getInstance()
    {
        static Singleton instance;
        return instance;
    }

    Singleton(Singleton const&) = delete;

private:
    Singleton()
    {
        std::cout << "Singleton constructed.\n";
    }
};

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
