#ifndef fiber_detail_Scheduler_h
#define fiber_detail_Scheduler_h

#include "Scheduler.fwd.h"

#include "Fiber.h"

#include "context/Context.h"

#include "intrusive/List.h"

#include <functional>

namespace fiber { namespace detail {

template<typename Traits>
class Scheduler {
public:
  typedef Fiber<Traits> fiber_type;
  typedef typename Traits::TimePoint TimePoint;

  Scheduler();
  ~Scheduler();

  static Scheduler* get();

  void run();

  void runTimer();

  void runInNewFiber(std::function<void()>&& task);

  [[ noreturn ]] void runFiber(fiber_type* fiber);

  void yield();

  template<typename Lockable>
  void wait(Lockable& conditionLock, WaitList<Traits>& waitList);

  template<typename Lockable>
  bool waitUntil(Lockable& conditionLock, WaitList<Traits>& waitList, const TimePoint& timePoint);

  void notifyOne(WaitList<Traits>& waitList);
  void notifyAll(WaitList<Traits>& waitList);

  void onTimer();

private:
  typedef typename Traits::SchedulerMutex Mutex;
  typedef typename Traits::SchedulerUniqueLock UniqueLock;

  void busyLoop(UniqueLock& lock);
  void idleLoop(UniqueLock& lock);

  void contextSwitch(context::Context* fromContext, UniqueLock& lock);
  void contextSwitch(context::Context* fromContext, context::Context toContext, UniqueLock& lock);
  UniqueLock postContextSwitch();

  fiber_type* getCurrentFiber() const;

  fiber_type* allocateFiber(std::function<void()>&& task, UniqueLock& lock);
  void deallocateFiber(fiber_type* fiber, UniqueLock& lock);

  void ready(fiber_type* fiber, UniqueLock& lock);
  void blocked(fiber_type* fiber, UniqueLock& lock);
  void unblocked(fiber_type* fiber, UniqueLock& lock);
  void timedWait(fiber_type* fiber, UniqueLock& lock);
  void timedOut(fiber_type* fiber, UniqueLock& lock);
  void dead(fiber_type* fiber, UniqueLock& lock);

  static thread_local context::Context t_threadContext;
  static thread_local Scheduler* t_scheduler;
  static thread_local fiber_type* t_currentFiber;

  Mutex m_mutex;

  AllList<Traits> m_allFibers;
  ReadyList<Traits> m_readyFibers;
  BlockedList<Traits> m_blockedFibers;
  TimedWaitList<Traits> m_timedWaitFibers;
  DeadList<Traits> m_deadFibers;

  typedef typename Traits::Idler Idler;
  Idler m_idler;

  typedef typename Traits::Timer Timer;
  Timer m_timer;
};

} } // namespace fiber::detail

#include "Scheduler.inl.h"

#endif // ifndef fiber_detail_Scheduler_h
