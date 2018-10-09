#include "StdService.h"

#include "assert/Assert.h"

#include <utility>

template<typename Traits>
inline fiber::detail::StdService<Traits>::StdService(std::size_t threadCount, std::function<void()>&& initTask) {
  VERIFY_GT(threadCount, 0U);

  m_scheduler.runInNewFiber(std::move(initTask));

  m_timerThread = std::thread(&Scheduler<Traits>::runTimer, std::ref(m_scheduler));

  while (threadCount--) {
    m_schedulerThreads.emplace_back(&Scheduler<Traits>::run, std::ref(m_scheduler));
  }
}

template<typename Traits>
inline fiber::detail::StdService<Traits>::~StdService() {
  while (!m_schedulerThreads.empty()) {
    m_schedulerThreads.back().join();
    m_schedulerThreads.pop_back();
  }

  m_timerThread.join();
}
