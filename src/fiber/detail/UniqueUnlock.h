#ifndef fiber_detail_UniqueUnlock_h
#define fiber_detail_UniqueUnlock_h

namespace fiber { namespace detail {

struct DeferUnlockT {};

constexpr DeferUnlockT DeferUnlock = DeferUnlockT();

template<typename Lockable>
class UniqueUnlock {
public:
  explicit UniqueUnlock(Lockable& lockable);
  UniqueUnlock(Lockable& lockable, DeferUnlockT);

  UniqueUnlock(const UniqueUnlock&) = delete;
  UniqueUnlock& operator=(UniqueUnlock&) = delete;

  ~UniqueUnlock();

  void lock();
  void unlock();

  bool ownsLock() const;

private:
  Lockable& m_lockable;

  bool m_unlocked;
};

} } // namespace fiber::detail

#include "UniqueUnlock.inl.h"

#endif // ifndef fiber_detail_UniqueUnlock_h
