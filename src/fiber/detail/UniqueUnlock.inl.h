#include "assert/Assert.h"

template<typename Lockable>
inline fiber::detail::UniqueUnlock<Lockable>::UniqueUnlock(Lockable& lockable)
  : m_lockable(lockable)
  , m_unlocked(false) {
  unlock();
}

template<typename Lockable>
inline fiber::detail::UniqueUnlock<Lockable>::UniqueUnlock(Lockable& lockable, DeferUnlockT)
  : m_lockable(lockable)
  , m_unlocked(false) {
}

template<typename Lockable>
inline fiber::detail::UniqueUnlock<Lockable>::~UniqueUnlock() {
  if (m_unlocked) {
    lock();
  }
}

template<typename Lockable>
inline void fiber::detail::UniqueUnlock<Lockable>::lock() {
  ASSERT(m_unlocked);
  m_lockable.lock();
  m_unlocked = false;
}

template<typename Lockable>
inline void fiber::detail::UniqueUnlock<Lockable>::unlock() {
  ASSERT(!m_unlocked);
  m_lockable.unlock();
  m_unlocked = true;
}

template<typename Lockable>
inline bool fiber::detail::UniqueUnlock<Lockable>::ownsLock() const {
  return !m_unlocked;
}
