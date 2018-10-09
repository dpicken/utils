#include "context/Context.h"

#include "assert/Assert.h"
#include "assert/verifyChildProcess.h"

#include <cstddef>
#include <signal.h>
#include <unistd.h>
#include <vector>

namespace {

int lastContext;

context::Context mainContext;
context::Context context1;
context::Context context2;

[[ noreturn ]] void context1Fn(void* arg) {
  VERIFY_EQ(arg, &context1);
  for (;;) {
    lastContext = 1;
    context::switchContext(&context1, context2);
    VERIFY_EQ(lastContext, 2);
  }
}

void context2Fn(void* arg) {
  VERIFY_EQ(arg, &context2);
  for (int it = 0; it != 3; ++it) {
    lastContext = 2;
    context::switchContext(&context2, context1);
    VERIFY_EQ(lastContext, 1);
  }
  lastContext = 2;
  context::switchContext(&context2, mainContext);
}

void testContextSwitch() {
  std::vector<std::byte> context1Stack(SIGSTKSZ);
  std::vector<std::byte> context2Stack(SIGSTKSZ);

  context1 = context::initContext(&context1Stack[0], context1Stack.size(), &context1Fn, &context1);
  context2 = context::initContext(&context2Stack[0], context2Stack.size(), &context2Fn, &context2);

  context::switchContext(&mainContext, context1);
  VERIFY_EQ(lastContext, 2);
}

void exitContextFn(void* arg) {
  VERIFY(arg == nullptr);
}

void testContextExit() {
  if (fork() == 0) {
    std::vector<std::byte> contextStack(SIGSTKSZ);
    context::Context context = context::initContext(&contextStack[0], contextStack.size(), &exitContextFn, nullptr);
    switchContext(&mainContext, context);
  } else {
    assert::verifyChildProcessExitsWithSuccess();
  }
}

[[ noreturn ]] void uncaughtExceptionFn(void* arg) {
  VERIFY(arg == nullptr);
  throw std::runtime_error("uncaughtExceptionFn");
}

void testUncaughtException() {
  if (fork() == 0) {
    std::vector<std::byte> contextStack(SIGSTKSZ);
    context::Context context = context::initContext(&contextStack[0], contextStack.size(), &uncaughtExceptionFn, nullptr);
    switchContext(&mainContext, context);
  } else {
    assert::verifyChildProcessAborts();
  }
}

}

int main(int, char**) {
  testContextSwitch();
  testContextExit();
  testUncaughtException();
  return 0;
}
