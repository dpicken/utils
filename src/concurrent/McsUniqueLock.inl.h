#include "McsMutex.h"

#include "assert/Assert.h"

inline concurrent::McsUniqueLock::McsUniqueLock(McsMutex& mutex)
  : m_mutex(mutex)
  , m_locked(false) {
  lock();
}

inline concurrent::McsUniqueLock::McsUniqueLock(McsMutex& mutex, DeferLockT)
  : m_mutex(mutex)
  , m_locked(false) {
}

inline concurrent::McsUniqueLock::~McsUniqueLock() {
  if (m_locked) {
    unlock();
  }
}

inline void concurrent::McsUniqueLock::lock() {
  ASSERT(!ownsLock());
  m_mutex.lock(m_owner);
  m_locked = true;
}

inline void concurrent::McsUniqueLock::unlock() {
  ASSERT(ownsLock());
  m_mutex.unlock(m_owner);
  m_locked = false;
  // Support re-locking.
  m_owner.reset();
}

inline bool concurrent::McsUniqueLock::ownsLock() const {
  return m_locked;
}
