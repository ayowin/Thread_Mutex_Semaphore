
#include "Semaphore.h"

#ifdef _WIN32
// windows
#include <windows.h>
#else
// linux
#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#endif

namespace Wz
{

struct SemaphoreHandle
{
#ifdef _WIN32
    // windows
    HANDLE pSemaphore;
#else
    // linux
    sem_t pSemaphore;
#endif
};

Semaphore::Semaphore(const int &value)
{
    semaphoreHandle = new SemaphoreHandle;
#ifdef _WIN32
    // windows
    semaphoreHandle->pSemaphore = CreateSemaphore(NULL,
                                                  value,
                                                  5000,
                                                  NULL);
    if (NULL == semaphoreHandle->pSemaphore)
#else
    // linux
    if (0 != sem_init(&(semaphoreHandle->pSemaphore),
                      1,
                      value))
#endif
    {
        delete semaphoreHandle;
        semaphoreHandle = NULL;
    }
}

Semaphore::~Semaphore()
{
    if (NULL != semaphoreHandle)
    {
#ifdef _WIN32
        // windows
        CloseHandle(semaphoreHandle->pSemaphore);
#else
        // linux
        sem_destroy(&(semaphoreHandle->pSemaphore));
#endif
        delete semaphoreHandle;
        semaphoreHandle = NULL;
    }
}

void Semaphore::wait()
{
    if (NULL == semaphoreHandle)
    {
        return;
    }

#ifdef _WIN32
    // windows
    WaitForSingleObject(semaphoreHandle->pSemaphore, INFINITE);
#else
    // linux
    sem_wait(&(semaphoreHandle->pSemaphore));
#endif
}

bool Semaphore::wait(const unsigned int &ms)
{
    if (NULL == semaphoreHandle)
    {
        return false;
    }

#ifdef _WIN32
    // windows
    DWORD result = WaitForSingleObject(semaphoreHandle->pSemaphore, ms);
    if (result == WAIT_OBJECT_0)
#else
    // linux
    timeval tv_now;
    gettimeofday(&tv_now, NULL);

    timespec ts;
    ts.tv_sec = tv_now.tv_sec;
    ts.tv_nsec = tv_now.tv_usec * 1000;

    int ns = ts.tv_nsec + (ms % 1000) * 1000000;
    ts.tv_nsec = ns % 1000000000;
    ts.tv_sec += ns / 1000000000;
    ts.tv_sec += ms / 1000;

    if (sem_timedwait(&(semaphoreHandle->pSemaphore), &ts) != 0)
#endif
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Semaphore::release()
{
    if (NULL == semaphoreHandle)
    {
        return;
    }

#ifdef _WIN32
    // windows
    ReleaseSemaphore(semaphoreHandle->pSemaphore, 1, NULL);
#else
    // linux
    sem_post(&(semaphoreHandle->pSemaphore));
#endif
}

} // namespace Wz