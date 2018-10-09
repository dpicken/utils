#ifndef fiber_detail_StdIdler_h
#define fiber_detail_StdIdler_h

#include <condition_variable>
#include <mutex>
#include <type_traits>

namespace fiber { namespace detail {

template<typename Traits>
class StdIdler {
public:
  typedef typename Traits::SchedulerUniqueLock SchedulerUniqueLock;

  StdIdler();

  void idle(SchedulerUniqueLock& lock);

  void wakeOne();

  void shutdown(SchedulerUniqueLock& lock);

private:
  typedef typename Traits::SchedulerMutex SchedulerMutex;
  typedef typename std::conditional<std::is_same<SchedulerMutex, std::mutex>::value, std::condition_variable, std::condition_variable_any>::type ConditionVariable;

  bool m_shutdown;

  ConditionVariable m_condition;
};

} } // namespace fiber::detail

#include "StdIdler.inl.h"

#endif // ifndef fiber_detail_StdIdler_h
