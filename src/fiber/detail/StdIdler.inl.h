#include "assert/Assert.h"

template<typename Traits>
inline fiber::detail::StdIdler<Traits>::StdIdler()
  : m_shutdown(false) {
}

template<typename Traits>
inline void fiber::detail::StdIdler<Traits>::idle(SchedulerUniqueLock& lock) {
  ASSERT(lock.owns_lock());
  if (m_shutdown) {
    return;
  }

  // Spurious wake-ups don't matter.
  m_condition.wait(lock);
}

template<typename Traits>
inline void fiber::detail::StdIdler<Traits>::wakeOne() {
  m_condition.notify_one();
}

template<typename Traits>
inline void fiber::detail::StdIdler<Traits>::shutdown(SchedulerUniqueLock& lock) {
  ASSERT(lock.owns_lock()); (void) lock;
  m_shutdown = true;
  m_condition.notify_all();
}
