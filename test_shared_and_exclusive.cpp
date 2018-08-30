#include "shared_recursive_mutex.h"

#include <cstdlib>
#include <atomic>
#include <thread>

int main()
{
    std::atomic_bool acquired1(false), acquired2(false), shutdown(false);

    shared_recursive_mutex m;
    m.lock();

    std::thread t1([&]() {
        m.lock_shared();
        acquired1.store(true);

        while (!shutdown.load()) {
            std::this_thread::yield();
        }

        m.unlock_shared();
    });

    std::thread t2([&]() {
        m.lock_shared();
        acquired2.store(true);

        while (!shutdown.load()) {
            std::this_thread::yield();
        }

        m.unlock_shared();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    if (acquired1.load() || acquired2.load()) {
        return EXIT_FAILURE;
    }

    m.unlock();

    while (!acquired1.load()) {
        std::this_thread::yield();
    }

    while (!acquired2.load()) {
        std::this_thread::yield();
    }

    shutdown.store(true);

    t1.join();
    t2.join();
}
