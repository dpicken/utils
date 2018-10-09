#ifndef fiber_detail_ConditionVariable_h
#define fiber_detail_ConditionVariable_h

#include "Fiber.h"

namespace fiber { namespace detail {

template<typename Traits>
class ConditionVariable {
public:
  typedef typename Traits::Duration Duration;
  typedef typename Traits::TimePoint TimePoint;

  ConditionVariable();
  ~ConditionVariable();

  template<typename Lockable>
  void wait(Lockable& lock);

  template<typename Lockable>
  bool waitFor(Lockable& lock, const Duration& duration);
  template<typename Lockable>
  bool waitUntil(Lockable& lock, const TimePoint& timePoint);

  void notifyOne();
  void notifyAll();

private:
  detail::WaitList<Traits> m_waitList;
};

} } // namespace fiber::detail

#include "ConditionVariable.inl.h"

#endif // ifndef fiber_detail_ConditionVariable_h
