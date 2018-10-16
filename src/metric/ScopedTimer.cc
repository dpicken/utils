#include "ScopedTimer.h"

#include <utility>

metric::ScopedTimer::ScopedTimer(std::function<void (Duration)>&& observer)
  : m_observer(std::move(observer))
  , m_startTime(Clock::now()) {
}

metric::ScopedTimer::~ScopedTimer() {
  Duration duration = Clock::now() - m_startTime;
  m_observer(duration);
}
