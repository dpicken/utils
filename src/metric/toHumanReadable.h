#ifndef metric_toHumanReadable_h
#define metric_toHumanReadable_h

#include <chrono>
#include <string>

namespace metric {

template<typename Duration>
std::string toHumanReadable(Duration duration);

template<> std::string toHumanReadable(std::chrono::hours duration);
template<> std::string toHumanReadable(std::chrono::minutes duration);
template<> std::string toHumanReadable(std::chrono::seconds duration);
template<> std::string toHumanReadable(std::chrono::milliseconds duration);
template<> std::string toHumanReadable(std::chrono::microseconds duration);
template<> std::string toHumanReadable(std::chrono::nanoseconds duration);

} // namespace metric

#endif // ifndef metric_toHumanReadable_h
