#ifndef fiber_detail_StdTimer_h
#define fiber_detail_StdTimer_h

#include "Scheduler.fwd.h"

#include <condition_variable>
#include <mutex>

namespace fiber { namespace detail {

template<typename Traits>
class StdTimer {
public:
  typedef typename Traits::Clock Clock;
  typedef typename Traits::TimePoint TimePoint;

  explicit StdTimer(Scheduler<Traits>* scheduler);

  void run();

  void restart(const TimePoint& expiryTime);

  void shutdown();

private:
  Scheduler<Traits>* m_scheduler;

  TimePoint m_expiryTime;

  bool m_shutdown;

  typedef std::mutex Mutex;
  typedef std::lock_guard<Mutex> LockGuard;
  typedef std::unique_lock<Mutex> UniqueLock;
  typedef std::condition_variable ConditionVariable;

  Mutex m_mutex;
  ConditionVariable m_condition;
};

} } // namespace fiber::detail

#include "StdTimer.inl.h"

#endif // ifndef fiber_detail_StdTimer_h
