#ifndef assert_SourceLocation_h
#define assert_SourceLocation_h

#include <iostream>

namespace assert {

class SourceLocation {
public:
  constexpr SourceLocation(const char* file, int line) noexcept
    : file(file)
    , line(line) {
  }

  const char* const file;
  const int line;
};

inline std::ostream& operator<<(std::ostream& os, const SourceLocation& sourceLocation) {
  return os << sourceLocation.file << ":" << sourceLocation.line;
}

} // namespace assert

#define MAKE_SOURCE_LOCATION assert::SourceLocation(__FILE__, __LINE__)

#endif // ifndef assert_SourceLocation_h
