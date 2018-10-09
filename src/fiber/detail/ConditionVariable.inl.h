#include "assert/Assert.h"

template<typename Traits>
inline fiber::detail::ConditionVariable<Traits>::ConditionVariable() {
}

template<typename Traits>
inline fiber::detail::ConditionVariable<Traits>::~ConditionVariable() {
  ASSERT(m_waitList.empty());
}

template<typename Traits>
template<typename Lockable>
inline void fiber::detail::ConditionVariable<Traits>::wait(Lockable& lock) {
  Scheduler<Traits>::get()->wait(lock, m_waitList);
}

template<typename Traits>
template<typename Lockable>
inline bool fiber::detail::ConditionVariable<Traits>::waitFor(Lockable& lock, const Duration& duration) {
  return TryWaitUntil(lock, Traits::Clock::now() + duration);
}

template<typename Traits>
template<typename Lockable>
inline bool fiber::detail::ConditionVariable<Traits>::waitUntil(Lockable& lock, const TimePoint& timePoint) {
  return Scheduler<Traits>::get()->waitUntil(lock, m_waitList, timePoint);
}

template<typename Traits>
inline void fiber::detail::ConditionVariable<Traits>::notifyOne() {
  Scheduler<Traits>::get()->notifyOne(m_waitList);
}

template<typename Traits>
inline void fiber::detail::ConditionVariable<Traits>::notifyAll() {
  Scheduler<Traits>::get()->notifyAll(m_waitList);
}
