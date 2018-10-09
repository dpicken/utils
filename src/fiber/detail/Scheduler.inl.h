#include "UniqueUnlock.h"

#include "assert/Assert.h"

#include <atomic>
#include <exception>
#include <mutex>

template<typename Traits>
thread_local context::Context fiber::detail::Scheduler<Traits>::t_threadContext;

template<typename Traits>
thread_local fiber::detail::Scheduler<Traits>* fiber::detail::Scheduler<Traits>::t_scheduler;

template<typename Traits>
thread_local fiber::detail::Fiber<Traits>* fiber::detail::Scheduler<Traits>::t_currentFiber;

template<typename Traits>
inline fiber::detail::Scheduler<Traits>::Scheduler()
  : m_timer(this) {
}

template<typename Traits>
inline fiber::detail::Scheduler<Traits>::~Scheduler() {
  ASSERT(m_allFibers.empty());
  ASSERT(m_readyFibers.empty());
  ASSERT(m_blockedFibers.empty());
  ASSERT(m_timedWaitFibers.empty());
  ASSERT(m_deadFibers.empty());
}

template<typename Traits>
inline fiber::detail::Scheduler<Traits>* fiber::detail::Scheduler<Traits>::get() {
  ASSERT_NE(t_scheduler, nullptr);
  return t_scheduler;
}

template<typename Traits>
inline void fiber::detail::Scheduler<Traits>::run() {
  t_scheduler = this;

  UniqueLock lock(m_mutex);
  while (!m_allFibers.empty()) {
    busyLoop(lock);
    idleLoop(lock);
  }

  m_idler.shutdown(lock);
  m_timer.shutdown();
}

template<typename Traits>
inline void fiber::detail::Scheduler<Traits>::busyLoop(UniqueLock& lock) {
  ASSERT(lock.owns_lock());
  while (!m_readyFibers.empty()) {
    contextSwitch(&t_threadContext, lock);
  }
}

template<typename Traits>
inline void fiber::detail::Scheduler<Traits>::idleLoop(UniqueLock& lock) {
  ASSERT(lock.owns_lock());
  while (m_readyFibers.empty() && !m_allFibers.empty()) {
    m_idler.idle(lock);
  }
}

template<typename Traits>
inline void fiber::detail::Scheduler<Traits>::runTimer() {
  m_timer.run();
}

template<typename Traits>
inline void fiber::detail::Scheduler<Traits>::runInNewFiber(std::function<void()>&& task) {
  UniqueLock lock(m_mutex, std::defer_lock);

  fiber_type* fiber = allocateFiber(std::move(task), lock);
  ready(fiber, lock);
}

template<typename Traits>
inline void fiber::detail::Scheduler<Traits>::runFiber(fiber_type* fiber) {
  UniqueLock lock = postContextSwitch();

  lock.unlock();
  fiber->task();
  lock.lock();

  dead(fiber, lock);
  contextSwitch(&fiber->context, lock);

  // The contextSwitch call should not return.
  std::terminate();
}

template<typename Traits>
inline void fiber::detail::Scheduler<Traits>::yield() {
  UniqueLock lock(m_mutex);

  fiber_type* fiber = getCurrentFiber();

  ready(fiber, lock);
  contextSwitch(&fiber->context, lock);
}

template<typename Traits>
template<typename Lockable>
inline void fiber::detail::Scheduler<Traits>::wait(Lockable& conditionLock, WaitList<Traits>& waitList) {
  // Ensure conditionLock is re-locked after scheduler lock is unlocked.
  UniqueUnlock<Lockable> conditionUnlock(conditionLock, DeferUnlock);

  UniqueLock lock(m_mutex);
  conditionUnlock.unlock();

  fiber_type* fiber = getCurrentFiber();
  waitList.push_back(*fiber);

  blocked(fiber, lock);
  contextSwitch(&fiber->context, lock);
}

template<typename Traits>
template<typename Lockable>
inline bool fiber::detail::Scheduler<Traits>::waitUntil(Lockable& conditionLock, WaitList<Traits>& waitList, const TimePoint& timePoint) {
  // Ensure conditionLock is re-locked after scheduler lock is unlocked.
  UniqueUnlock<Lockable> conditionUnlock(conditionLock, DeferUnlock);

  UniqueLock lock(m_mutex);
  conditionUnlock.unlock();

  fiber_type* fiber = getCurrentFiber();
  waitList.push_back(*fiber);

  fiber->waitUntil = timePoint;

  timedWait(fiber, lock);
  contextSwitch(&fiber->context, lock);

  if (fiber->timedWaitListNode.onList()) {
    ASSERT(!fiber->timedOut);
    ASSERT(!fiber->waitListNode.onList());
    fiber->timedWaitListNode.erase();
  } else if (fiber->waitListNode.onList()) {
    ASSERT(fiber->timedOut);
    ASSERT(!fiber->timedWaitListNode.onList());
    fiber->waitListNode.erase();
  } else {
    ASSERT(!fiber->waitListNode.onList());
    ASSERT(!fiber->timedWaitListNode.onList());
  }

  return fiber->timedOut;
}

template<typename Traits>
inline void fiber::detail::Scheduler<Traits>::notifyOne(WaitList<Traits>& waitList) {
  UniqueLock lock(m_mutex);

  if (waitList.empty()) {
    return;
  }

  fiber_type* fiber = &waitList.front();
  waitList.pop_front();
  unblocked(fiber, lock);
}

template<typename Traits>
inline void fiber::detail::Scheduler<Traits>::notifyAll(WaitList<Traits>& waitList) {
  UniqueLock lock(m_mutex);

  while (!waitList.empty()) {
    fiber_type* fiber = &waitList.front();
    waitList.pop_front();
    unblocked(fiber, lock);
  }
}

template<typename Traits>
inline void fiber::detail::Scheduler<Traits>::onTimer() {
  UniqueLock lock(m_mutex);

  while (!m_timedWaitFibers.empty() && m_timedWaitFibers.front().waitUntil <= Traits::Clock::now()) {
    fiber_type* fiber = &m_timedWaitFibers.front();
    if (fiber->blockedListNode.onList()) {
      timedOut(fiber, lock);
    } else {
      m_timedWaitFibers.pop_front();
    }
  }

  if (!m_timedWaitFibers.empty()) {
    m_timer.restart(m_timedWaitFibers.front().waitUntil);
  }
}

template<typename Traits>
inline void fiber::detail::Scheduler<Traits>::contextSwitch(context::Context* fromContext, UniqueLock& lock) {
  ASSERT(lock.owns_lock());

  if (!m_readyFibers.empty()) {
    fiber_type* toFiber = &m_readyFibers.front();
    m_readyFibers.pop_front();
    if (&toFiber->context != fromContext) {
      t_currentFiber = toFiber;
      contextSwitch(fromContext, toFiber->context, lock);
    }
  } else if (fromContext != &t_threadContext) {
    t_currentFiber = nullptr;
    contextSwitch(fromContext, t_threadContext, lock);
  }
}

template<typename Traits>
inline void fiber::detail::Scheduler<Traits>::contextSwitch(context::Context* fromContext, context::Context toContext, UniqueLock& lock) {
  ASSERT(lock.owns_lock());
  lock.release();

  // Prevent the compiler optimizing out the lock release/assignment (which will occur on different stacks).
  std::atomic_signal_fence(std::memory_order_acquire);
  context::switchContext(fromContext, toContext);
  std::atomic_signal_fence(std::memory_order_release);

  lock = postContextSwitch();
}

template<typename Traits>
inline typename fiber::detail::Scheduler<Traits>::UniqueLock fiber::detail::Scheduler<Traits>::postContextSwitch() {
  UniqueLock lock(m_mutex, std::adopt_lock);
  if (!m_deadFibers.empty()) {
    ASSERT_EQ(m_deadFibers.size(), 1U);
    fiber_type* deadFiber = &m_deadFibers.back();
    m_deadFibers.pop_back();
    deallocateFiber(deadFiber, lock);
  }
  return lock;
}

template<typename Traits>
inline fiber::detail::Fiber<Traits>* fiber::detail::Scheduler<Traits>::getCurrentFiber() const {
  ASSERT_NE(t_currentFiber, nullptr);
  return t_currentFiber;
}

template<typename Traits>
inline fiber::detail::Fiber<Traits>* fiber::detail::Scheduler<Traits>::allocateFiber(std::function<void()>&& task, UniqueLock& lock) {
  ASSERT(!lock.owns_lock()); (void) lock;
  fiber_type* fiber = new fiber_type(this, std::move(task));

  lock.lock();
  m_allFibers.push_back(*fiber);
  return fiber;
}

template<typename Traits>
inline void fiber::detail::Scheduler<Traits>::deallocateFiber(fiber_type* fiber, UniqueLock& lock) {
  ASSERT(lock.owns_lock()); (void) lock;
  fiber->allListNode.erase();

  UniqueUnlock unlock(lock);
  delete fiber;
}

template<typename Traits>
inline void fiber::detail::Scheduler<Traits>::ready(fiber_type* fiber, UniqueLock& lock) {
  ASSERT(lock.owns_lock()); (void) lock;
  m_readyFibers.push_back(*fiber);
  m_idler.wakeOne();
}

template<typename Traits>
inline void fiber::detail::Scheduler<Traits>::blocked(fiber_type* fiber, UniqueLock& lock) {
  ASSERT(lock.owns_lock()); (void) lock;
  m_blockedFibers.push_back(*fiber);
}

template<typename Traits>
inline void fiber::detail::Scheduler<Traits>::unblocked(fiber_type* fiber, UniqueLock& lock) {
  ASSERT(lock.owns_lock());
  fiber->blockedListNode.erase();
  ready(fiber, lock);
}

template<typename Traits>
inline void fiber::detail::Scheduler<Traits>::timedWait(fiber_type* fiber, UniqueLock& lock) {
  ASSERT(lock.owns_lock());

  typename TimedWaitList<Traits>::iterator it = m_timedWaitFibers.begin();
  while (it != m_timedWaitFibers.end() && it->waitUntil <= fiber->waitUntil) {
    ++it;
  }

  if (it == m_timedWaitFibers.begin()) {
    m_timer.restart(fiber->waitUntil);
  }

  fiber->timedOut = false;
  m_timedWaitFibers.insert(it, *fiber);

  blocked(fiber, lock);
}

template<typename Traits>
inline void fiber::detail::Scheduler<Traits>::timedOut(fiber_type* fiber, UniqueLock& lock) {
  ASSERT(lock.owns_lock());
  fiber->timedOut = true;
  fiber->timedWaitListNode.erase();
  unblocked(fiber, lock);
}

template<typename Traits>
inline void fiber::detail::Scheduler<Traits>::dead(fiber_type* fiber, UniqueLock& lock) {
  ASSERT(lock.owns_lock()); (void) lock;
  m_deadFibers.push_back(*fiber);
}
