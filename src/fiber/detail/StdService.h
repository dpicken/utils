#ifndef fiber_detail_StdService_h
#define fiber_detail_StdService_h

#include "Scheduler.h"

#include <cstddef>
#include <functional>
#include <thread>
#include <vector>

namespace fiber { namespace detail {

template<typename Traits>
class StdService {
public:
  StdService(std::size_t threadCount, std::function<void()>&& initTask);

  ~StdService();

private:
  Scheduler<Traits> m_scheduler;

  std::thread m_timerThread;

  std::vector<std::thread> m_schedulerThreads;
};

} } // namespace fiber::detail

#include "StdService.inl.h"

#endif // ifndef fiber_detail_StdService_h
