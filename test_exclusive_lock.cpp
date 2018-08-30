#include "shared_recursive_mutex.h"

#include <cstdlib>
#include <chrono>
#include <atomic>
#include <thread>

int main()
{
    std::atomic_bool acquired(false);

    shared_recursive_mutex m;
    m.lock();

    std::thread t([&]() {
        m.lock();
        acquired.store(true);
        m.unlock();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    if (acquired.load()) {
        return EXIT_FAILURE;
    }

    m.unlock();

    while (!acquired.load()) {
        std::this_thread::yield();
    }

    t.join();
}
