#ifndef concurrent_McsUniqueLock_h
#define concurrent_McsUniqueLock_h

#include "McsMutex.fwd.h"

#include "detail/McsLockOwner.h"

namespace concurrent {

struct DeferLockT {};

constexpr DeferLockT DeferLock = DeferLockT();

class McsUniqueLock {
public:
  McsUniqueLock(McsMutex& mutex);
  McsUniqueLock(McsMutex& mutex, DeferLockT);

  ~McsUniqueLock();

  void lock();

  void unlock();

  bool ownsLock() const;

private:
  McsMutex& m_mutex;

  detail::McsLockOwner m_owner;

  bool m_locked;
};

} // namespace concurrent

#include "McsUniqueLock.inl.h"

#endif // ifndef concurrent_McsUniqueLock_h
