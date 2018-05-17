#ifndef assert_Assert_h
#define assert_Assert_h

#include "SourceLocation.h"

#include <iostream>
#include <sstream>
#include <type_traits>

/**
 * Compile-time asserts that provide a descriptive failure message.
 */
#define STATIC_ASSERT(assertion) static_assert((assertion), "static assertion failed: " #assertion)
#define STATIC_ASSERT_WITH_OPERATOR(lhs, rhs, operator) static_assert((lhs) operator (rhs), "static assertion failed: " #lhs " " #operator " " #rhs)
#define STATIC_ASSERT_EQ(lhs, rhs) STATIC_ASSERT_WITH_OPERATOR(lhs, rhs, ==)
#define STATIC_ASSERT_NE(lhs, rhs) STATIC_ASSERT_WITH_OPERATOR(lhs, rhs, !=)
#define STATIC_ASSERT_LT(lhs, rhs) STATIC_ASSERT_WITH_OPERATOR(lhs, rhs, <)
#define STATIC_ASSERT_GT(lhs, rhs) STATIC_ASSERT_WITH_OPERATOR(lhs, rhs, >)
#define STATIC_ASSERT_LE(lhs, rhs) STATIC_ASSERT_WITH_OPERATOR(lhs, rhs, <=)
#define STATIC_ASSERT_GE(lhs, rhs) STATIC_ASSERT_WITH_OPERATOR(lhs, rhs, >=)

/**
 * Non-debug asserts that provide a descriptive failure message.
 *
 * Arguments are evaluated once.
 */
#define VERIFY(assertion) (assertion) ? ((void) 0) : assert::assertionFailed(MAKE_SOURCE_LOCATION, #assertion)
#define VERIFY_EQ(lhs, rhs) assert::eq::verify(MAKE_SOURCE_LOCATION, #lhs, lhs, #rhs, rhs);
#define VERIFY_NE(lhs, rhs) assert::ne::verify(MAKE_SOURCE_LOCATION, #lhs, lhs, #rhs, rhs);
#define VERIFY_LE(lhs, rhs) assert::le::verify(MAKE_SOURCE_LOCATION, #lhs, lhs, #rhs, rhs);
#define VERIFY_GE(lhs, rhs) assert::ge::verify(MAKE_SOURCE_LOCATION, #lhs, lhs, #rhs, rhs);
#define VERIFY_LT(lhs, rhs) assert::lt::verify(MAKE_SOURCE_LOCATION, #lhs, lhs, #rhs, rhs);
#define VERIFY_GT(lhs, rhs) assert::gt::verify(MAKE_SOURCE_LOCATION, #lhs, lhs, #rhs, rhs);

/**
 * Debug-only asserts that provide a descriptive failure message.
 *
 * Arguments are evaluated once.
 */
#ifdef NDEBUG
#define ASSERT(assertion) ((void) 0)
#define ASSERT_EQ(lhs, rhs) ((void) 0)
#define ASSERT_NE(lhs, rhs) ((void) 0)
#define ASSERT_LE(lhs, rhs) ((void) 0)
#define ASSERT_GE(lhs, rhs) ((void) 0)
#define ASSERT_LT(lhs, rhs) ((void) 0)
#define ASSERT_GT(lhs, rhs) ((void) 0)
#else
#define ASSERT(assertion) VERIFY(assertion)
#define ASSERT_EQ(lhs, rhs) VERIFY_EQ(lhs, rhs)
#define ASSERT_NE(lhs, rhs) VERIFY_NE(lhs, rhs)
#define ASSERT_LE(lhs, rhs) VERIFY_LE(lhs, rhs)
#define ASSERT_GE(lhs, rhs) VERIFY_GE(lhs, rhs)
#define ASSERT_LT(lhs, rhs) VERIFY_LT(lhs, rhs)
#define ASSERT_GT(lhs, rhs) VERIFY_GT(lhs, rhs)
#endif // ifdef DEBUG

namespace assert {

[[noreturn]] inline void assertionFailed(const SourceLocation& sourceLocation, const char* const assertion) {
  std::cerr
      << std::endl << std::endl
      << sourceLocation << ": assertion failed: " << assertion
      << std::endl;
  std::abort();
}

template<typename T1, typename T2>
inline void assertionFailed(const SourceLocation& sourceLocation, const char* lhsName, const T1& lhsValue, const char* rhsName, const T2& rhsValue, const char* operatorStr) {
  std::ostringstream assertionOS;
  assertionOS << lhsName;
  if constexpr (std::is_null_pointer<T1>::value != true) {
    assertionOS << " (" << lhsValue << ")";
  }
  assertionOS << " " << operatorStr << " " << rhsName;
  if constexpr (std::is_null_pointer<T2>::value != true) {
    assertionOS << " (" << rhsValue << ")";
  }
  assertionFailed(sourceLocation, assertionOS.str().c_str());
}

#define MAKE_ASSERT(assertName, operator)\
struct assertName {\
  template<typename T1, typename T2>\
  static inline void verify(const SourceLocation& sourceLocation, const char* lhsName, const T1& lhsValue, const char* rhsName, const T2& rhsValue) {\
    if (!(lhsValue operator rhsValue)) {\
      assertionFailed(sourceLocation, lhsName, lhsValue, rhsName, rhsValue, #operator);\
    }\
  }\
}

MAKE_ASSERT(eq, ==);
MAKE_ASSERT(ne, !=);
MAKE_ASSERT(le, <=);
MAKE_ASSERT(ge, >=);
MAKE_ASSERT(lt, <);
MAKE_ASSERT(gt, >);

} // namespace assert

#endif // ifndef assert_Assert_h
