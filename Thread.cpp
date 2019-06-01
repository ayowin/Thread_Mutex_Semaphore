
#include "Thread.h"

#ifdef _WIN32
// windows
#include <windows.h>
#include <process.h>
#else
// linux
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#endif

namespace Wz
{

#ifdef _WIN32
// windows
struct ThreadHandle
{
    HANDLE pThread;
};

unsigned int static __stdcall threadCallBack(void *p)
{
    Thread *t = (Thread *)p;
    t->run();
    return 0;
}
#else
// linux
struct ThreadHandle
{
    pthread_t pThread;
};

static void *threadCallBack(void *p)
{
    Thread *t = (Thread *)p;
    t->run();
    return NULL;
}
#endif

Thread::Thread()
    : runnable(NULL), threadHandle(NULL)
{
}

Thread::Thread(Runnable runnable)
    : runnable(runnable), threadHandle(NULL)
{
}

Thread::~Thread()
{
    if (threadHandle)
    {
        delete threadHandle;
        threadHandle = NULL;
    }
}

void Thread::setRunnable(Runnable runnable)
{
    this->runnable = runnable;
}

void Thread::run()
{
    if (NULL != runnable)
    {
        runnable();
    }
}

void Thread::start()
{
    threadHandle = new ThreadHandle;
#ifdef _WIN32
    // windows
    unsigned int threadId;
    threadHandle->pThread = (HANDLE)_beginthreadex(NULL,
                                                   0,
                                                   threadCallBack,
                                                   this,
                                                   0,
                                                   &threadId);
    if (0 == threadHandle->pThread)
#else
    // linux
    if (0 != pthread_create(&(threadHandle->pThread),
                            NULL,
                            threadCallBack,
                            this))
#endif
    {
        delete threadHandle;
        threadHandle = NULL;

        printf("Thread: thread create failed.");
        return;
    }
}

void Thread::join()
{
    if (threadHandle)
    {
#ifdef _WIN32
        // windows
        WaitForSingleObject(threadHandle->pThread, INFINITE);
        CloseHandle(threadHandle->pThread);
#else
        // linux
        pthread_join(threadHandle->pThread, NULL);
#endif
        delete threadHandle;
        threadHandle = NULL;
    }
}

void Thread::sleepS(const unsigned int &s)
{
#ifdef _WIN32
    // windows
    Sleep(s * 1000);
#else
    // linux
    sleep(s);
#endif
}

void Thread::sleepMs(const unsigned int &ms)
{
#ifdef _WIN32
    // windows
    Sleep(ms);
#else
    // linux
    usleep(ms * 1000);
#endif
}

} // namespace Wz