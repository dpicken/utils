#include "detail/NullLockable.h"
#include "detail/Scheduler.h"

#include "assert/Assert.h"

#include <utility>

template<typename Traits>
inline fiber::Service<Traits>::Service(std::size_t threadCount, std::function<void()>&& initTask)
  : m_impl(threadCount, std::move(initTask)) {
}

template<typename Traits>
inline void fiber::Service<Traits>::runInNewFiber(std::function<void()>&& task) {
  detail::Scheduler<Traits>::get()->runInNewFiber(std::move(task));
}

template<typename Traits>
inline void fiber::Service<Traits>::yield() {
  detail::Scheduler<Traits>::get()->yield();
}

template<typename Traits>
inline void fiber::Service<Traits>::sleepFor(Duration duration) {
  sleepUntil(Traits::Clock::now() + duration);
}

template<typename Traits>
inline void fiber::Service<Traits>::sleepUntil(TimePoint timePoint) {
  ConditionVariable cv;
  detail::NullLockable lockable;
  lockable.lock();
  bool timedOut = cv.waitUntil(lockable, timePoint);
  ASSERT(timedOut); (void) timedOut;
}
