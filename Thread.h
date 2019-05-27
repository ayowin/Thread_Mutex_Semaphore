
#ifndef _THREAD_H_
#define _THREAD_H_

#include <iostream>

namespace BZL
{

typedef void (*Runnable)();

struct ThreadHandle;

class Thread
{
public:
    Thread();
    Thread(Runnable runnable);
    virtual ~Thread();

    virtual void run();

    void start();
    void join();

    static void sleepS(const unsigned int& s);
    static void sleepMs(const unsigned int& ms);

private:
    Runnable runnable;
    ThreadHandle* threadHandle;
};

}

#endif