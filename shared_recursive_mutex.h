#pragma once

#include <mutex>
#include <condition_variable>
#include <thread>
#include <set>

class shared_recursive_mutex
{
public:
    shared_recursive_mutex();
    ~shared_recursive_mutex();

    void lock();
    bool try_lock();
    void unlock();

    void lock_shared();
    bool try_lock_shared();
    void unlock_shared();

private:
    bool try_lock(std::unique_lock<std::mutex> &);
    bool try_lock_shared(std::unique_lock<std::mutex> &);

    std::mutex m;
    std::condition_variable cv;
    std::multiset<std::thread::id> shared_locks;
    std::thread::id exclusive_lock;
    std::size_t exclusive_lock_depth;
};
