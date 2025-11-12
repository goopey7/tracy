#ifndef __TRACYMUTEX_HPP__
#define __TRACYMUTEX_HPP__

#if defined _MSC_VER

#  include <shared_mutex>

namespace tracy
{
using TracyMutex = std::shared_mutex;
}

#elif defined __wii__
#  include <ogc/mutex.h>
namespace tracy
{
class TracyMutex
{
    mutex_t m_mutex;

public:
    TracyMutex() { LWP_MutexInit( &m_mutex, false ); }
    ~TracyMutex() { LWP_MutexDestroy( m_mutex ); }
    void lock() { LWP_MutexLock( m_mutex ); }
    void unlock() { LWP_MutexUnlock( m_mutex ); }
    bool try_lock() { return LWP_MutexTryLock( m_mutex ) == 0; }
};
}
#else

#include <mutex>

namespace tracy
{
using TracyMutex = std::mutex;
}

#endif

#endif
