#include "testVerifyChildProcess.h"

#include "assert/verifyChildProcess.h"

#include <cstdlib>

#include <unistd.h>

void assert::test::testVerifyChildProcess() {
  if (fork() == 0) {
    std::exit(0);
  } else {
    assert::verifyChildProcessExitsWithSuccess();
  }

  if (fork() == 0) {
    std::exit(1);
  } else {
    assert::verifyChildProcessExitsWithFailure();
  }

  if (fork() == 0) {
    std::exit(0);
  } else {
    assert::verifyChildProcessExitsWithStatus(0);
  }

  if (fork() == 0) {
    std::exit(1);
  } else {
    assert::verifyChildProcessExitsWithStatus(1);
  }

  if (fork() == 0) {
    std::abort();
  } else {
    assert::verifyChildProcessAborts();
  }
}
