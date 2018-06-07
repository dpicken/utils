#include "concurrent/McsMutex.h"
#include "concurrent/McsUniqueLock.h"

#include "assert/Assert.h"

#include <chrono>
#include <functional>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

namespace {

void testScopedLock() {
  concurrent::McsMutex mutex;

  {
    concurrent::McsUniqueLock lock(mutex);
    VERIFY(lock.ownsLock());
  }

  {
    concurrent::McsUniqueLock lock(mutex);
    VERIFY(lock.ownsLock());

    lock.unlock();
    VERIFY(!lock.ownsLock());
  }

  {
    concurrent::McsUniqueLock lock(mutex);
    VERIFY(lock.ownsLock());

    lock.unlock();
    VERIFY(!lock.ownsLock());

    lock.lock();
    VERIFY(lock.ownsLock());
  }

  {
    concurrent::McsUniqueLock lock(mutex, concurrent::DeferLock);
    VERIFY(!lock.ownsLock());
  }

  {
    concurrent::McsUniqueLock lock(mutex, concurrent::DeferLock);
    VERIFY(!lock.ownsLock());

    lock.lock();
    VERIFY(lock.ownsLock());
  }
}

void testSynchronization() {
  if (std::thread::hardware_concurrency() < 2) {
    return;
  }

  concurrent::McsMutex mutex;
  int x = 0;

  std::thread thread1([&]() {
    concurrent::McsUniqueLock lock(mutex);
    while (x == 0) {
      lock.unlock();
      lock.lock();
    }
    VERIFY_EQ(x, 1);
    ++x;
  });

  std::thread thread2([&]() {
    concurrent::McsUniqueLock lock(mutex);
    std::this_thread::sleep_for(500ms);
    VERIFY_EQ(x, 0);
    ++x;
  });

  thread1.join();
  thread2.join();

  VERIFY_EQ(x, 2);
}

typedef std::promise<std::chrono::nanoseconds> AsyncDurationResult;

void repeatLockAndUnlock(AsyncDurationResult& result, concurrent::McsMutex& mutex, unsigned int lockCount) {
  concurrent::McsUniqueLock lock(mutex);
  lock.unlock();

  auto startTime = std::chrono::high_resolution_clock::now();
  for (unsigned int i = 0; i < lockCount; ++i) {
    lock.lock();
    lock.unlock();
  }
  auto endTime = std::chrono::high_resolution_clock::now();

  result.set_value(std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime));
}

void testLockThroughput(unsigned int threadCount) {
  VERIFY_LT(threadCount, std::thread::hardware_concurrency());

  unsigned int perThreadLockCount = 1000000;
  concurrent::McsMutex mutex;
  concurrent::McsUniqueLock lock(mutex);

  std::vector<AsyncDurationResult> results(threadCount);
  for (unsigned int i = 0; i < threadCount; ++i) {
    std::thread thread(&repeatLockAndUnlock, std::ref(results[i]), std::ref(mutex), perThreadLockCount);
    thread.detach();
  }

  // HACK; ~Wait for threads to arrive at lock.
  std::this_thread::sleep_for(500ms);
  lock.unlock();

  std::chrono::nanoseconds totalNs(0);
  for (AsyncDurationResult& result : results) {
    totalNs += result.get_future().get();
  }

  std::cout << __func__ << ": "
      << "threads=" << threadCount
      << ", mean lock time=" << (totalNs.count() / (perThreadLockCount * threadCount)) << "ns"
      << std::endl;
}

void testLockThroughput() {
  for (unsigned int threadCount = 1; threadCount < std::thread::hardware_concurrency(); ++threadCount) {
    testLockThroughput(threadCount);
  }
}

} // namespace

int main(int, char**) {
  testScopedLock();
  testSynchronization();
  testLockThroughput();
  return 0;
}
