#ifndef fiber_detail_Fiber_h
#define fiber_detail_Fiber_h

#include "Scheduler.fwd.h"

#include "context/Context.h"
#include "intrusive/List.h"
#include "intrusive/ListNode.h"

#include <cstddef>
#include <functional>
#include <vector>

namespace fiber { namespace detail {

template<typename Traits>
class Fiber {
public:
  Fiber(Scheduler<Traits>* scheduler, std::function<void()>&& task);

  void run();

  Scheduler<Traits>* const scheduler;

  std::function<void()> task;

  std::vector<std::byte> stack;

  context::Context context;

  intrusive::ListNode allListNode;
  intrusive::ListNode readyListNode;
  intrusive::ListNode blockedListNode;
  intrusive::ListNode waitListNode;
  intrusive::ListNode timedWaitListNode;
  intrusive::ListNode deadListNode;

  typename Traits::TimePoint waitUntil;
  bool timedOut;
};

template<typename Traits>
using AllList = INTRUSIVE_LIST(Fiber<Traits>, allListNode);

template<typename Traits>
using ReadyList = INTRUSIVE_LIST(Fiber<Traits>, readyListNode);

template<typename Traits>
using BlockedList = INTRUSIVE_LIST(Fiber<Traits>, blockedListNode);

template<typename Traits>
using WaitList = INTRUSIVE_LIST(Fiber<Traits>, waitListNode);

template<typename Traits>
using TimedWaitList = INTRUSIVE_LIST(Fiber<Traits>, timedWaitListNode);

template<typename Traits>
using DeadList = INTRUSIVE_LIST(Fiber<Traits>, deadListNode);

} } // namespace fiber::detail

#include "Fiber.inl.h"

#endif // ifndef fiber_detail_Fiber_h
