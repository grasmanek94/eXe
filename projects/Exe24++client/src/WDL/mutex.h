/*
    WDL - mutex.h
    Copyright (C) 2005 and later, Cockos Incorporated
   
    This software is provided 'as-is', without any express or implied
    warranty.  In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
       claim that you wrote the original software. If you use this software
       in a product, an acknowledgment in the product documentation would be
       appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
       misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
      
*/

/*


  This file provides a simple class that abstracts a mutex or critical section object.
  On Windows it uses CRITICAL_SECTION, on everything else it uses pthread's mutex library.
  It simulates the Critical Section behavior on non-Windows, as well (meaning a thread can 
  safely Enter the mutex multiple times, provided it Leaves the same number of times)
  
*/

#ifndef _WDL_MUTEX_H_
#define _WDL_MUTEX_H_

#ifdef _WIN32
#include <windows.h>
#else

#include <unistd.h>
// define this if you wish to use carbon critical sections on OS X
// #define WDL_MAC_USE_CARBON_CRITSEC

#ifdef WDL_MAC_USE_CARBON_CRITSEC
#include <Carbon/Carbon.h>
#else
#include <pthread.h>
#endif

#ifdef __APPLE__
#include <libkern/OSAtomic.h>
#endif

#endif

#include "wdltypes.h"

class WDL_Mutex {
  public:
    WDL_Mutex() 
    {
#ifdef _WIN32
      InitializeCriticalSection(&m_cs);
#else
#ifdef WDL_MAC_USE_CARBON_CRITSEC
      MPCreateCriticalRegion(&m_cr);
#else
      m_ownerthread=0;
      m_lockcnt=0;
      pthread_mutex_init(&m_mutex,NULL);
#endif
#endif
    }
    ~WDL_Mutex()
    {
#ifdef _WIN32
      DeleteCriticalSection(&m_cs);
#else
#ifdef WDL_MAC_USE_CARBON_CRITSEC
      MPDeleteCriticalRegion(m_cr);
#else
      pthread_mutex_destroy(&m_mutex);
#endif
#endif
    }

    void Enter()
    {
#ifdef _WIN32
      EnterCriticalSection(&m_cs);
#else
#ifdef WDL_MAC_USE_CARBON_CRITSEC
      MPEnterCriticalRegion(m_cr,kDurationForever);
#else
      pthread_t tt=pthread_self();
      if (m_ownerthread==tt) m_lockcnt++;
      else
      {
        pthread_mutex_lock(&m_mutex);
        m_ownerthread=tt;
        m_lockcnt=0;
      }
#endif
#endif
    }

    void Leave()
    {
#ifdef _WIN32
      LeaveCriticalSection(&m_cs);
#else
#ifdef WDL_MAC_USE_CARBON_CRITSEC
      MPExitCriticalRegion(m_cr);
#else
      if (--m_lockcnt < 0)
      {
        m_ownerthread=0;
        pthread_mutex_unlock(&m_mutex);
      }
#endif
#endif
    }

  private:
#ifdef _WIN32
  CRITICAL_SECTION m_cs;
#else
#ifdef WDL_MAC_USE_CARBON_CRITSEC
  MPCriticalRegionID m_cr;
#else
  pthread_mutex_t m_mutex;
  pthread_t m_ownerthread;
  int m_lockcnt;
#endif
#endif

} WDL_FIXALIGN;

class WDL_MutexLock {
public:
  WDL_MutexLock(WDL_Mutex *m) : m_m(m) { if (m) m->Enter(); }
  ~WDL_MutexLock() { if (m_m) m_m->Leave(); }
private:
  WDL_Mutex *m_m;
} WDL_FIXALIGN;

class WDL_SharedMutex 
{
  public:
    WDL_SharedMutex() { m_sharedcnt=0; }
    ~WDL_SharedMutex() { }

    void LockExclusive()  // note: the calling thread must NOT have any shared locks, or deadlock WILL occur
    { 
      m_mutex.Enter(); 
#ifdef _WIN32
      while (m_sharedcnt>0) Sleep(1);
#else
      while (m_sharedcnt>0) usleep(100);		
#endif
    }
    void UnlockExclusive() { m_mutex.Leave(); }

    void LockShared() 
    { 
      m_mutex.Enter();
#ifdef _WIN32
      InterlockedIncrement(&m_sharedcnt);
#elif defined (__APPLE__)
      OSAtomicIncrement32(&m_sharedcnt);
#else
      m_cntmutex.Enter();
      m_sharedcnt++;
      m_cntmutex.Leave();
#endif

      m_mutex.Leave();
    }
    void UnlockShared()
    {
#ifdef _WIN32
      InterlockedDecrement(&m_sharedcnt);
#elif defined(__APPLE__)
      OSAtomicDecrement32(&m_sharedcnt);
#else
      m_cntmutex.Enter();
      m_sharedcnt--;
      m_cntmutex.Leave();
#endif
    }

  private:
    WDL_Mutex m_mutex;
#ifdef _WIN32
    LONG m_sharedcnt;
#elif defined(__APPLE__)
    int32_t m_sharedcnt;
#else
    WDL_Mutex m_cntmutex;
    int m_sharedcnt;
#endif
} WDL_FIXALIGN;


#endif
