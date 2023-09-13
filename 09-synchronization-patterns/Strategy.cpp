#include <iostream>
#include <memory>
#include <utility>

class Strategy {
public:
    virtual void operator()() = 0;
    virtual ~Strategy() = default;
};

class Context {
public:
    Context()
        : strategy_ { nullptr }
    {
    }

    void setStrategy(std::shared_ptr<Strategy> s)
    {
        strategy_ = std::move(s);
    }

    void strategy()
    {
        if (strategy_) {
            (*strategy_)();
        }
    }

private:
    std::shared_ptr<Strategy> strategy_;
};

class Strategy1 final : public Strategy {
public:
    void operator()() override
    {
        std::cout << "Foo\n";
    }
};

class Strategy2 final : public Strategy {
public:
    void operator()() override
    {
        std::cout << "Bar\n";
    }
};

class Strategy3 final : public Strategy {
public:
    void operator()() override
    {
        std::cout << "FooBaz\n";
    }
};

int main()
{
    Context con;
    con.strategy();

    con.setStrategy(std::make_shared<Strategy1>());
    con.strategy();

    con.setStrategy(std::make_shared<Strategy2>());
    con.strategy();

    con.setStrategy(std::make_shared<Strategy3>());
    con.strategy();

    return 0;
}
