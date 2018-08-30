#include "shared_recursive_mutex.h"

int main()
{
    shared_recursive_mutex m;
    m.lock();
    m.lock_shared();
    m.lock();
    m.lock_shared();
    m.unlock_shared();
    m.unlock();
    m.unlock_shared();
    m.unlock();
}
