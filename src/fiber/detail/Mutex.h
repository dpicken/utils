#ifndef fiber_detail_Mutex_h
#define fiber_detail_Mutex_h

#include "Semaphore.h"

namespace fiber { namespace detail {

template<typename Traits>
class Mutex {
public:
  Mutex();

  ~Mutex();

  void lock();

  bool tryLock();

  void unlock();

private:
  Semaphore<Traits> m_semaphore;
};

} } // namespace fiber::detail

#include "Mutex.inl.h"

#endif // ifndef fiber_detail_Mutex_h
