#ifndef error_throwErrnoSystemError_h
#define error_throwErrnoSystemError_h

#include "makeErrnoErrorCode.h"

#include <system_error>

namespace error {

[[noreturn]] inline void throwErrnoSystemError() {
  throw std::system_error(makeErrnoErrorCode());
}

inline void throwErrnoSystemErrorIfEq(int lhs, int rhs) {
  if (lhs == rhs) {
    throwErrnoSystemError();
  }
}

inline void throwErrnoSystemErrorIfNe(int lhs, int rhs) {
  if (lhs != rhs) {
    throwErrnoSystemError();
  }
}

template<typename T>
inline void throwErrnoSystemErrorIfNull(const T* value) {
  if (value == nullptr) {
    throwErrnoSystemError();
  }
}

} // namespace error

#endif // ifndef error_throwErrnoSystemError_h
