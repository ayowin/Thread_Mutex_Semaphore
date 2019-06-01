
#ifndef _MUTEX_H_
#define _MUTEX_H_

#include <iostream>

namespace Wz
{

struct MutexHandle;

class Mutex
{
public:
    Mutex();
    ~Mutex();

    void lock();
    void unlock();

private:
    MutexHandle *mutexHandle;
};

} // namespace Wz

#endif