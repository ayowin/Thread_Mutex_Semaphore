
#include <iostream>
#include "Thread.h"
#include "Mutex.h"
#include "Semaphore.h"

using namespace std;

BZL::Mutex mutex;
BZL::Semaphore semaphore(1);

unsigned int tmp = 0;

void thread1()
{
    // mutex.lock();
    for (int i = 0; i < 100; i++)
    {
        if(0 == i%10)
        {
            semaphore.release();
        }

        tmp += i;
        printf("threa1: %d\n", tmp);
        BZL::Thread::sleepMs(200);
    }

    // mutex.unlock();
}

class Thread2 : public BZL::Thread
{
public:
    void run()
    {
        // mutex.lock();

        for (int i = 0; i < 100; i++)
        {
            semaphore.wait();

            tmp -= i;
            printf("threa2: %d\n", tmp);
            BZL::Thread::sleepMs(300);
        }

        // mutex.unlock();
    }
};

int main(int argc, char **argv)
{
    printf("------Thread------\n");

    BZL::Thread t1(&thread1);
    t1.start();

    Thread2 t2;
    t2.start();

    getchar();

    return 0;
}