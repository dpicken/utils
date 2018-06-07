#include "McsMutex.h"

#include "detail/McsLockOwner.h"

#include "assert/Assert.h"

concurrent::McsMutex::McsMutex()
  : m_queueTail(nullptr) {
    STATIC_ASSERT(QueueTail::is_always_lock_free);
}

concurrent::McsMutex::~McsMutex() {
  ASSERT_EQ(m_queueTail.load(std::memory_order_relaxed), nullptr);
}

void concurrent::McsMutex::lock(detail::McsLockOwner& newLockOwner) {
  ASSERT(newLockOwner.notified.load(std::memory_order_relaxed) == false);
  ASSERT_EQ(newLockOwner.nextLockOwner.load(std::memory_order_relaxed), nullptr);

  // Attempt an uncontended acquisition of the mutex (acquire/release performed on m_queueTail).
  detail::McsLockOwner* contendingLockOwner = m_queueTail.exchange(&newLockOwner, std::memory_order_acquire);
  if (contendingLockOwner == nullptr) {
    return;
  }

  // Enqueue behind the lock owner with which we('ll) contend.
  contendingLockOwner->nextLockOwner.store(&newLockOwner, std::memory_order_relaxed);

  // Perform a contended acquisition of the mutex (acquire/release on newLockOwner.notified).
  while (!newLockOwner.notified.load(std::memory_order_relaxed)) {
    // TODO: instruct CPU to pause/yield?
  }
  newLockOwner.notified.load(std::memory_order_acquire);
}

void concurrent::McsMutex::unlock(detail::McsLockOwner& currentLockOwner) {
  // Attempt an uncontended release of the mutex (acquire/release performed on m_queueTail).
  detail::McsLockOwner* desiredQueueTail = &currentLockOwner;
  if (m_queueTail.compare_exchange_strong(desiredQueueTail, nullptr, std::memory_order_release)) {
    return;
  }

  // Spin until the new lock owner has enqueued itself.
  detail::McsLockOwner* newLockOwner;
  while ((newLockOwner = currentLockOwner.nextLockOwner.load(std::memory_order_relaxed)) == nullptr) {
    // TODO: instruct CPU to pause/yield?
  }

  // Perform a contended release of the mutex (acquire/release performed on newLockOwner.notified).
  newLockOwner->notified.store(true, std::memory_order_release);
}
