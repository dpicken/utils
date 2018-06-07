#include "assert/Assert.h"

inline concurrent::detail::McsLockOwner::McsLockOwner()
  : nextLockOwner(nullptr)
  , notified(false) {
    STATIC_ASSERT(LockOwner::is_always_lock_free);
    STATIC_ASSERT(Notified::is_always_lock_free);
}

inline void concurrent::detail::McsLockOwner::reset() {
  this->~McsLockOwner();
  new(this) McsLockOwner();
}
