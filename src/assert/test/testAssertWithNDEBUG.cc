#include "testAssertWithNDEBUG.h"

#undef NDEBUG
#define NDEBUG
#include "assert/Assert.h"

void assert::test::testAssertWithNDEBUG() {
  ASSERT(true);
  ASSERT(false);

  ASSERT_EQ(true, true);
  ASSERT_EQ(true, false);

  ASSERT_NE(true, false);
  ASSERT_NE(true, true);

  ASSERT_LE(0, 1);
  ASSERT_LE(1, 1);
  ASSERT_LE(2, 1);

  ASSERT_GE(2, 1);
  ASSERT_GE(1, 1);
  ASSERT_GE(0, 1);

  ASSERT_LT(0, 1);
  ASSERT_LT(0, 0);

  ASSERT_GT(1, 0);
  ASSERT_GT(0, 0);
}
