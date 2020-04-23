#ifndef error_makeErrnoErrorCode_h
#define error_makeErrnoErrorCode_h

#include <cerrno>
#include <system_error>

namespace error {

inline std::error_code makeErrnoErrorCode() {
  return std::error_code(errno, std::generic_category());
}

inline std::error_code makeErrnoErrorCodeIfEq(int lhs, int rhs) {
  if (lhs == rhs) {
    return makeErrnoErrorCode();
  }
  return std::error_code();
}

inline std::error_code makeErrnoErrorCodeIfNe(int lhs, int rhs) {
  if (lhs != rhs) {
    return makeErrnoErrorCode();
  }
  return std::error_code();
}

template<typename T>
inline std::error_code makeErrnoErrorCodeIfNull(const T* value) {
  if (value == nullptr) {
    return makeErrnoErrorCode();
  }
  return std::error_code();
}

} // namespace error

#endif // ifndef error_makeErrnoErrorCode_h
