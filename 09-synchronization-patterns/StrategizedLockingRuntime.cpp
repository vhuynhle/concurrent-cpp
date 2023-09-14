#include <iostream>
#include <mutex>
#include <shared_mutex>

class Lock {
public:
    virtual void lock() const = 0;
    virtual void unlock() const = 0;

    Lock() = default;
    virtual ~Lock() = default;
    Lock(const Lock&) = delete;
    Lock(Lock&&) = delete;
    Lock& operator=(const Lock&) = delete;
    Lock& operator=(Lock&&) = delete;
};

class StrategizedLocking {
public:
    explicit StrategizedLocking(Lock& l)
        : lock { l }
    {
        lock.lock();
    }

    ~StrategizedLocking()
    {
        lock.unlock();
    }

private:
    Lock& lock;
};

class NoLock final : public Lock {
public:
    void lock() const override
    {
        std::cout << "NoLock::lock()\n";
    }

    void unlock() const override
    {
        std::cout << "NoLock::unlock()\n";
    };
};

class ExclusiveLock : public Lock {
public:
    void lock() const override
    {
        std::cout << "ExlusiveLock::lock()\n";
        mutex.lock();
    }

    void unlock() const override
    {
        std::cout << "ExlusiveLock::unlock()\n";
        mutex.unlock();
    }

private:
    mutable std::mutex mutex;
};

class SharedLock : public Lock {
public:
    void lock() const override
    {
        std::cout << "SharedLock::lock_shared()\n";
        sharedMutex.lock();
    }

    void unlock() const override
    {
        std::cout << "SharedLock::unlock_shared()\n";
        sharedMutex.unlock_shared();
    }

private:
    mutable std::shared_mutex sharedMutex;
};

int main()
{
    NoLock noLock;
    StrategizedLocking noLockStrategy { noLock };
    {
        ExclusiveLock exLock;
        StrategizedLocking exclusiveLockStrategy { exLock };

        {
            SharedLock sharedLock;
            StrategizedLocking sharedLockStrategy { sharedLock };
        }
    }
    return 0;
}
