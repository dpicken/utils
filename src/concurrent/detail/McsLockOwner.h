#ifndef concurrent_detail_McsLockOwner_h
#define concurrent_detail_McsLockOwner_h

#include "McsLockOwner.fwd.h"

#include <atomic>

namespace concurrent { namespace detail {

struct McsLockOwner {
  McsLockOwner();

  void reset();

  typedef std::atomic<McsLockOwner*> LockOwner;
  LockOwner nextLockOwner;

  // g++8.1 claims this is not always lock free:
  //typedef std::atomic<bool> Notified;
  typedef std::atomic<int> Notified;
  Notified notified;
};

} } // namespace concurrent::detail

#include "McsLockOwner.inl.h"

#endif // ifndef concurrent_detail_McsLockOwner_h
