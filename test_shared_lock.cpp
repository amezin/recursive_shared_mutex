#include "shared_recursive_mutex.h"

#include <cstdlib>
#include <atomic>
#include <thread>

int main()
{
    std::atomic_bool acquired(false);

    shared_recursive_mutex m;
    m.lock_shared();

    std::thread t([&]() {
        m.lock_shared();
        acquired.store(true);
        m.unlock_shared();
    });

    while (!acquired.load()) {
        std::this_thread::yield();
    }

    t.join();
    m.unlock_shared();
}
