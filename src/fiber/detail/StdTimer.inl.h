#include "UniqueUnlock.h"

template<typename Traits>
inline fiber::detail::StdTimer<Traits>::StdTimer(Scheduler<Traits>* scheduler)
  : m_scheduler(scheduler)
  , m_expiryTime(TimePoint::max())
  , m_shutdown(false) {
}

template<typename Traits>
inline void fiber::detail::StdTimer<Traits>::run() {
  UniqueLock lock(m_mutex);

  for (;;) {
    while (!m_shutdown && Clock::now() < m_expiryTime) {
      // g++ 8.1 doesn't handle waiting on TimePoint::max().
      if (m_expiryTime == TimePoint::max()) {
        m_condition.wait(lock);
      } else {
        m_condition.wait_until(lock, m_expiryTime);
      }
    }

    if (m_shutdown) {
      return;
    }

    m_expiryTime = TimePoint::max();

    UniqueUnlock<UniqueLock> unlock(lock);
    m_scheduler->onTimer();
  }
}

template<typename Traits>
inline void fiber::detail::StdTimer<Traits>::restart(const TimePoint& expiryTime)
{
  LockGuard lock(m_mutex);
  m_expiryTime = expiryTime;
  m_condition.notify_one();
}

template<typename Traits>
inline void fiber::detail::StdTimer<Traits>::shutdown() {
  LockGuard lock(m_mutex);
  m_shutdown = true;
  m_condition.notify_one();
}
