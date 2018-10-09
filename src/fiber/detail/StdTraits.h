#ifndef fiber_detail_StdTraits_h
#define fiber_detail_StdTraits_h

#include "StdIdler.h"
#include "StdService.h"
#include "StdTimer.h"

#include "context/Context.h"

#include <chrono>

namespace fiber { namespace detail {

extern "C" void stdFiberStartContext(void* arg);

struct StdTraits {
  constexpr static context::StartContextFn fiberStartContextFn = stdFiberStartContext;

  typedef std::mutex SchedulerMutex;
  typedef std::unique_lock<SchedulerMutex> SchedulerUniqueLock;

  typedef std::chrono::steady_clock Clock;
  typedef typename Clock::duration Duration;
  typedef typename Clock::time_point TimePoint;

  typedef StdService<StdTraits> Service;

  typedef StdIdler<StdTraits> Idler;
  typedef StdTimer<StdTraits> Timer;
};

} } // namespace fiber::detail

#endif // ifndef fiber_detail_StdTraits_h
