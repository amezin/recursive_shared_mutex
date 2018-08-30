#include "shared_recursive_mutex.h"

#include <cassert>

shared_recursive_mutex::shared_recursive_mutex()
    : exclusive_lock_depth(0)
{
}

shared_recursive_mutex::~shared_recursive_mutex()
{
    assert(exclusive_lock == std::thread::id());
    assert(exclusive_lock_depth == 0);
    assert(shared_locks.empty());
}

bool shared_recursive_mutex::try_lock(std::unique_lock<std::mutex> &)
{
    auto this_id = std::this_thread::get_id();

    if (shared_locks.size() > shared_locks.count(this_id)) {
        return false;
    }

    if (exclusive_lock != std::thread::id() && exclusive_lock != this_id) {
        return false;
    }

    exclusive_lock = this_id;
    exclusive_lock_depth++;
    return true;
}

bool shared_recursive_mutex::try_lock()
{
    std::unique_lock<std::mutex> l(m);
    return try_lock(l);
}

void shared_recursive_mutex::lock()
{
    std::unique_lock<std::mutex> l(m);
    while (!try_lock(l)) {
        cv.wait(l);
    }
}

void shared_recursive_mutex::unlock()
{
    std::unique_lock<std::mutex> l(m);
    assert(exclusive_lock == std::this_thread::get_id());

    exclusive_lock_depth--;
    if (!exclusive_lock_depth) {
        exclusive_lock = std::thread::id();
    }
    cv.notify_all();
}

bool shared_recursive_mutex::try_lock_shared(std::unique_lock<std::mutex> &)
{
    auto this_id = std::this_thread::get_id();

    if (exclusive_lock != std::thread::id() && exclusive_lock != this_id) {
        return false;
    }

    shared_locks.insert(this_id);
    return true;
}

bool shared_recursive_mutex::try_lock_shared()
{
    std::unique_lock<std::mutex> l(m);
    return try_lock_shared(l);
}

void shared_recursive_mutex::lock_shared()
{
    std::unique_lock<std::mutex> l(m);
    while (!try_lock_shared(l)) {
        cv.wait(l);
    }
}

void shared_recursive_mutex::unlock_shared()
{
    std::unique_lock<std::mutex> l(m);
    auto i = shared_locks.find(std::this_thread::get_id());
    assert(i != shared_locks.end());
    shared_locks.erase(i);
    cv.notify_all();
}
