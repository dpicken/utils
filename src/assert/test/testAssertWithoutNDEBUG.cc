#include "testAssertWithoutNDEBUG.h"

#include "expectChildAborts.h"

#undef NDEBUG
#include "assert/Assert.h"

#include <unistd.h>

void assert::test::testAssertWithoutNDEBUG() {
  ASSERT(true);
  if (fork() == 0) {
    ASSERT(false);
  } else {
    expectChildAborts();
  }

  ASSERT_EQ(true, true);
  if (fork() == 0) {
    ASSERT_EQ(true, false);
  } else {
    expectChildAborts();
  }

  ASSERT_NE(true, false);
  if (fork() == 0) {
    ASSERT_NE(true, true);
  } else {
    expectChildAborts();
  }

  ASSERT_LE(0, 1);
  ASSERT_LE(1, 1);
  if (fork() == 0) {
    ASSERT_LE(2, 1);
  } else {
    expectChildAborts();
  }

  ASSERT_GE(2, 1);
  ASSERT_GE(1, 1);
  if (fork() == 0) {
    ASSERT_GE(0, 1);
  } else {
    expectChildAborts();
  }

  ASSERT_LT(0, 1);
  if (fork() == 0) {
    ASSERT_LT(0, 0);
  } else {
    expectChildAborts();
  }

  ASSERT_GT(1, 0);
  if (fork() == 0) {
    ASSERT_GT(0, 0);
  } else {
    expectChildAborts();
  }
}
