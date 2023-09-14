#include <iostream>
#include <mutex>
#include <shared_mutex>

template <typename LOCK>
class StrategizedLocking {
public:
    explicit StrategizedLocking(LOCK& l)
        : lock { l }
    {
        lock.lock();
    }

    ~StrategizedLocking()
    {
        lock.unlock();
    }

private:
    LOCK& lock;
};

class NoLock {
public:
    void lock() const
    {
        std::cout << "NoLock::lock()\n";
    }

    void unlock() const
    {
        std::cout << "NoLock::unlock()\n";
    };
};

class ExclusiveLock {
public:
    void lock() const
    {
        std::cout << "ExlusiveLock::lock()\n";
        mutex.lock();
    }

    void unlock() const
    {
        std::cout << "ExlusiveLock::unlock()\n";
        mutex.unlock();
    }

private:
    mutable std::mutex mutex;
};

class SharedLock {
public:
    void lock() const
    {
        std::cout << "SharedLock::lock_shared()\n";
        sharedMutex.lock();
    }

    void unlock() const
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
