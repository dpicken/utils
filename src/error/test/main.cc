#include "error/makeErrnoErrorCode.h"
#include "error/throwErrnoSystemError.h"

#include "assert/Assert.h"

#include <type_traits>

int main(int, char**) {
  VERIFY_EQ(&error::makeErrnoErrorCode().category(), &std::generic_category());
  VERIFY_EQ(error::makeErrnoErrorCode().value(), 0);

  errno = E2BIG;
  VERIFY_EQ(&error::makeErrnoErrorCode().category(), &std::generic_category());
  VERIFY_EQ(error::makeErrnoErrorCode().value(), static_cast<typename std::underlying_type<std::errc>::type>(std::errc::argument_list_too_long));

  errno = EAFNOSUPPORT;
  VERIFY_EQ(&error::makeErrnoErrorCode().category(), &std::generic_category());
  VERIFY_EQ(error::makeErrnoErrorCode().value(), static_cast<typename std::underlying_type<std::errc>::type>(std::errc::address_family_not_supported));

  try {
    error::throwErrnoSystemErrorIfEq(0, 0);
    VERIFY(false);
  } catch (const std::system_error& e) {
    VERIFY_EQ(e.code().value(), EAFNOSUPPORT);
  }
  try {
    error::throwErrnoSystemErrorIfEq(0, 1);
  } catch (const std::system_error&) {
    VERIFY(false);
  }

  try {
    error::throwErrnoSystemErrorIfNe(0, 1);
    VERIFY(false);
  } catch (const std::system_error& e) {
    VERIFY_EQ(e.code().value(), EAFNOSUPPORT);
  }
  try {
    error::throwErrnoSystemErrorIfNe(0, 0);
  } catch (const std::system_error&) {
    VERIFY(false);
  }

  try {
    int* a = nullptr;
    error::throwErrnoSystemErrorIfNull(a);
    VERIFY(false);
  } catch (const std::system_error& e) {
    VERIFY_EQ(e.code().value(), EAFNOSUPPORT);
  }
  try {
    int a;
    error::throwErrnoSystemErrorIfNull(&a);
  } catch (const std::system_error&) {
    VERIFY(false);
  }

  return 0;
}
