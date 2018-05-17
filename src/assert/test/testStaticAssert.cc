#include "testStaticAssert.h"

#include "assert/Assert.h"

void assert::test::testStaticAssert() {
  STATIC_ASSERT(true);
  STATIC_ASSERT_EQ(true, true);
  STATIC_ASSERT_NE(true, false);
  STATIC_ASSERT_LE(0, 1);
  STATIC_ASSERT_LE(1, 1);
  STATIC_ASSERT_GE(2, 1);
  STATIC_ASSERT_GE(1, 1);
  STATIC_ASSERT_LT(0, 1);
  STATIC_ASSERT_GT(1, 0);

  // TODO: No way to test these without failing the build.
  //STATIC_ASSERT(false);
  //STATIC_ASSERT_EQ(true, false);
  //STATIC_ASSERT_NE(true, true);
  //STATIC_ASSERT_LE(2, 1);
  //STATIC_ASSERT_GE(0, 1);
  //STATIC_ASSERT_LT(1, 1);
  //STATIC_ASSERT_GT(0, 0);
}
