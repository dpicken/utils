#ifndef metric_ScopedTimer_h
#define metric_ScopedTimer_h

#include <chrono>
#include <functional>

namespace metric {

class ScopedTimer {
public:
  using Clock = std::chrono::high_resolution_clock;
  using Duration = Clock::duration;
  using TimePoint = Clock::time_point;

  explicit ScopedTimer(std::function<void (Duration)>&& observer);

  ~ScopedTimer();

private:
  std::function<void (Duration)> m_observer;

  const TimePoint m_startTime;
};

} // namespace metric

#endif // ifndef metric_ScopedTimer_h
