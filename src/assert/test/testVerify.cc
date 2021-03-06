#include "testVerify.h"

#include "assert/Assert.h"
#include "assert/verifyChildProcess.h"

#include <unistd.h>

void assert::test::testVerify() {
  VERIFY(true);
  if (fork() == 0) {
    VERIFY(false);
  } else {
    assert::verifyChildProcessAborts();
  }

  VERIFY_EQ(true, true);
  if (fork() == 0) {
    VERIFY_EQ(true, false);
  } else {
    assert::verifyChildProcessAborts();
  }

  VERIFY_NE(true, false);
  if (fork() == 0) {
    VERIFY_NE(true, true);
  } else {
    assert::verifyChildProcessAborts();
  }

  VERIFY_LE(0, 1);
  VERIFY_LE(1, 1);
  if (fork() == 0) {
    VERIFY_LE(2, 1);
  } else {
    assert::verifyChildProcessAborts();
  }

  VERIFY_GE(2, 1);
  VERIFY_GE(1, 1);
  if (fork() == 0) {
    VERIFY_GE(0, 1);
  } else {
    assert::verifyChildProcessAborts();
  }

  VERIFY_LT(0, 1);
  if (fork() == 0) {
    VERIFY_LT(0, 0);
  } else {
    assert::verifyChildProcessAborts();
  }

  VERIFY_GT(1, 0);
  if (fork() == 0) {
    VERIFY_GT(0, 0);
  } else {
    assert::verifyChildProcessAborts();
  }
}
