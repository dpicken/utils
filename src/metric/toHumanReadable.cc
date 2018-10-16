#include "toHumanReadable.h"

#include <sstream>

namespace {

template<typename Duration>
std::string toString(const Duration& duration, const char* units) {
  std::ostringstream os;
  os << duration.count() << " " << units;
  return os.str();
}

}

template<>
std::string metric::toHumanReadable(std::chrono::hours duration) {
  return toString(duration, "hours");
}

template<>
std::string metric::toHumanReadable(std::chrono::minutes duration) {
  if (duration.count() > 60 * 24) {
    return toHumanReadable(std::chrono::duration_cast<std::chrono::minutes>(duration));
  }
  return toString(duration, "minutes");
}

template<>
std::string metric::toHumanReadable(std::chrono::seconds duration) {
  if (duration.count() > 60 * 60) {
    return toHumanReadable(std::chrono::duration_cast<std::chrono::minutes>(duration));
  }
  return toString(duration, "seconds");
}

template<>
std::string metric::toHumanReadable(std::chrono::milliseconds duration) {
  if (duration.count() > 10000) {
    return toHumanReadable(std::chrono::duration_cast<std::chrono::seconds>(duration));
  }
  return toString(duration, "ms");
}

template<>
std::string metric::toHumanReadable(std::chrono::microseconds duration) {
  if (duration.count() > 10000) {
    return toHumanReadable(std::chrono::duration_cast<std::chrono::milliseconds>(duration));
  }
  return toString(duration, "us");
}

template<>
std::string metric::toHumanReadable(std::chrono::nanoseconds duration) {
  if (duration.count() > 10000) {
    return toHumanReadable(std::chrono::duration_cast<std::chrono::microseconds>(duration));
  }
  return toString(duration, "ns");
}
