#include "fiber/StdService.h"

#include <chrono>
#include <cstddef>
#include <functional>

using namespace std::chrono_literals;

namespace {

template<std::size_t ThreadCount, std::size_t TaskCount, std::size_t IterationCount>
struct TestTraits {
  constexpr static std::size_t threadCount = ThreadCount;
  constexpr static std::size_t taskCount = TaskCount;
  constexpr static std::size_t iterationCount = IterationCount;
};

template<typename FiberService, typename TestTraits>
void runFiberTest(std::function<void()>&& initTask) {
  FiberService service(TestTraits::threadCount, std::move(initTask));
}

template<typename FiberService, typename TestTraits>
void testTask(const std::function<void()>& task) {
  runFiberTest<FiberService, TestTraits>([&task]() {
    for (std::size_t i = 0; i != TestTraits::taskCount; ++i) {
      FiberService::runInNewFiber(std::function<void()>(task));
    }
  });
}

template<typename FiberService, typename TestTraits>
void testYield() {
  testTask<FiberService, TestTraits>([] {
    std::size_t iterationCount = TestTraits::iterationCount;
    while (iterationCount-- != 0) {
      FiberService::yield();
    }
  });
}

template<typename FiberService, typename TestTraits>
void testSleep() {
  testTask<FiberService, TestTraits>([] {
    std::size_t iterationCount = TestTraits::iterationCount;
    while (iterationCount-- != 0) {
      FiberService::sleepFor(10ms);
    }
  });
}

template<typename FiberService, typename TestTraits>
void testUncontendedMutex() {
  typedef typename FiberService::Mutex Mutex;

  testTask<FiberService, TestTraits>([] {
    Mutex mutex;
    std::size_t iterationCount = TestTraits::iterationCount;
    while (iterationCount-- != 0) {
      std::unique_lock<Mutex> lock(mutex);
    }
  });
}

template<typename FiberService, typename TestTraits>
void testContendedMutex() {
  typedef typename FiberService::Mutex Mutex;

  Mutex mutex;
  testTask<FiberService, TestTraits>([&mutex] () mutable -> void {
    std::size_t iterationCount = TestTraits::iterationCount;
    while (iterationCount-- != 0) {
      std::unique_lock<Mutex> lock(mutex);
      FiberService::yield();
      lock.unlock();
    }
  });
}

template<typename FiberService, std::size_t threadCount, std::size_t taskCount>
void test() {
  testYield<FiberService,
      TestTraits<threadCount, taskCount, 100> >();
  testSleep<FiberService,
      TestTraits<threadCount, taskCount, 10> >();
  testUncontendedMutex<FiberService,
      TestTraits<threadCount, taskCount, 100> >();
  testContendedMutex<FiberService,
      TestTraits<threadCount, taskCount, 100> >();
}

template<typename FiberService, std::size_t threadCount>
void test() {
  test<FiberService, threadCount, 0>();
  test<FiberService, threadCount, 1>();
  test<FiberService, threadCount, 2>();
  test<FiberService, threadCount, 3>();
  test<FiberService, threadCount, 61>();
  test<FiberService, threadCount, 64>();
  test<FiberService, threadCount, 256>();
  test<FiberService, threadCount, 257>();
  test<FiberService, threadCount, 1021>();
  test<FiberService, threadCount, 1024>();
}

} // namespace

int main(int, char**) {
  test<fiber::StdService, 1>();
  test<fiber::StdService, 2>();
  test<fiber::StdService, 4>();
  test<fiber::StdService, 8>();
  return 0;
}
