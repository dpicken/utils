#ifndef fiber_Service_h
#define fiber_Service_h

#include "detail/ConditionVariable.h"
#include "detail/Mutex.h"
#include "detail/Semaphore.h"

namespace fiber {

template<typename Traits>
class Service {
public:
  typedef detail::ConditionVariable<Traits> ConditionVariable;
  typedef detail::Mutex<Traits> Mutex;
  typedef detail::Mutex<Traits> Semaphore;

  typedef typename Traits::Duration Duration;
  typedef typename Traits::TimePoint TimePoint;

  Service(std::size_t threadCount, std::function<void()>&& initTask);

  static void runInNewFiber(std::function<void()>&& task);

  static void yield();

  static void sleepFor(Duration duration);

  static void sleepUntil(TimePoint timePoint);

private:
  typename Traits::Service m_impl;
};

} // namespace fiber

#include "Service.inl.h"

#endif // ifndef fiber_Service_h
