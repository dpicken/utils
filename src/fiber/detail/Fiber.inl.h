#include "Scheduler.h"

#include "context/Context.h"

#include <signal.h>

template<typename Traits>
inline fiber::detail::Fiber<Traits>::Fiber(Scheduler<Traits>* s, std::function<void()>&& t)
  : scheduler(s)
  , task(t)
  , stack(SIGSTKSZ)
  , context(context::initContext(&stack[0], stack.size(), Traits::fiberStartContextFn, this)) {
}

template<typename Traits>
inline void fiber::detail::Fiber<Traits>::run() {
  scheduler->runFiber(this);
}
