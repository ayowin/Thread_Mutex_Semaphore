
#include "Mutex.h"

#ifdef _WIN32
// windows
#include <windows.h>
#else
// linux
#include <pthread.h>
#endif

namespace Wz
{

struct MutexHandle
{
#ifdef _WIN32
    // windows
    HANDLE pMutex;
#else
    // linux
    pthread_mutex_t pMutex;
#endif
};

Mutex::Mutex()
{
    mutexHandle = new MutexHandle;
#ifdef _WIN32
    // windows
    mutexHandle->pMutex = CreateMutex(NULL,
                                      true,
                                      NULL);
    if (NULL == mutexHandle->pMutex)
    {
        delete mutexHandle;
        mutexHandle = NULL;
        printf("Mutex: mutex create failed.");
    }
    ReleaseMutex(mutexHandle->pMutex);
#else
    // linux
    if (0 != pthread_mutex_init(&(mutexHandle->pMutex),
                                NULL))
    {
        delete mutexHandle;
        mutexHandle = NULL;
        printf("Mutex: mutex create failed.");
    }
#endif
}

Mutex::~Mutex()
{
    if (NULL != mutexHandle)
    {
#ifdef _WIN32
        // windows
        CloseHandle(mutexHandle->pMutex);
#else
        // linux
        pthread_mutex_destroy(&(mutexHandle->pMutex));
#endif
        delete mutexHandle;
        mutexHandle = NULL;
    }
}

void Mutex::lock()
{
    if (NULL == mutexHandle)
    {
        return;
    }
#ifdef _WIN32
    // windows
    WaitForSingleObject(mutexHandle->pMutex, INFINITE);
#else
    // linux
    pthread_mutex_lock(&(mutexHandle->pMutex));
#endif
}

void Mutex::unlock()
{
    if (NULL == mutexHandle)
    {
        return;
    }
#ifdef _WIN32
    // windows
    ReleaseMutex(mutexHandle->pMutex);
#else
    // linux
    pthread_mutex_unlock(&(mutexHandle->pMutex));
#endif
}

} // namespace Wz