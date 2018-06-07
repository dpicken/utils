#ifndef concurrent_McsMutex_h
#define concurrent_McsMutex_h

#include "detail/McsLockOwner.fwd.h"

#include <atomic>

namespace concurrent {

/**
 * A mutex based on an MCS spin lock.
 *
 * Each CPU core spins on a distinct variable (and cache line).
 *
 * Intended to be locked/unlocked with UniqueMcsLock.
 */
class McsMutex {
public:
  McsMutex();

  ~McsMutex();

  void lock(detail::McsLockOwner& nextLockOwner);

  void unlock(detail::McsLockOwner& currentLockOwner);

private:
  typedef std::atomic<detail::McsLockOwner*> QueueTail;
  QueueTail m_queueTail;
};

} // namespace concurrent

#endif // ifndef concurrent_McsMutex_h
