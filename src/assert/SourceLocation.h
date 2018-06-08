#ifndef assert_SourceLocation_h
#define assert_SourceLocation_h

#include <iostream>

namespace assert {

class SourceLocation {
public:
  constexpr SourceLocation(const char* file, int line) noexcept
    : m_file(file)
    , m_line(line) {
  }

  friend std::ostream& operator<<(std::ostream&, const SourceLocation&);

private:
  const char* const m_file;
  const int m_line;
};

inline std::ostream& operator<<(std::ostream& os, const SourceLocation& sourceLocation) {
  return os << sourceLocation.m_file << ":" << sourceLocation.m_line;
}

} // namespace assert

#define MAKE_SOURCE_LOCATION assert::SourceLocation(__FILE__, __LINE__)

#endif // ifndef assert_SourceLocation_h
