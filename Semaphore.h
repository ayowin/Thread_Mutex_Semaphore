
#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#include <iostream>

namespace Wz
{

struct SemaphoreHandle;

class Semaphore
{
public:
    Semaphore(const int &value = 0);
    ~Semaphore();

    void wait();                       // semaphore - 1
    bool wait(const unsigned int &ms); // semaphore - 1 , timeout ms
    void release();                    // semaphore + 1

private:
    SemaphoreHandle *semaphoreHandle;
};

} // namespace Wz

#endif