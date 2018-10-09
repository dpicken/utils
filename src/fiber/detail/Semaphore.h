#ifndef fiber_detail_Semaphore_h
#define fiber_detail_Semaphore_h

#include "ConditionVariable.h"

#include "concurrent/McsMutex.h"
#include "concurrent/McsUniqueLock.h"

namespace fiber { namespace detail {

template<typename Traits>
class Semaphore {
public:
  typedef unsigned int size_type;

  Semaphore(size_type count);

  void wait();

  bool tryWait();

  void notifyOne();

private:
  typedef concurrent::McsMutex Mutex;
  typedef concurrent::McsUniqueLock UniqueLock;

  Mutex m_mutex;

  size_type m_count;

  ConditionVariable<Traits> m_countIncremented;
};

} } // namespace fiber::detail

#include "Semaphore.inl.h"

#endif // ifndef fiber_detail_Semaphore_h
