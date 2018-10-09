#ifndef fiber_detail_NullLockable_h
#define fiber_detail_NullLockable_h

namespace fiber { namespace detail {

struct NullLockable {
  void lock() {
  }

  void unlock() {
  }
};

} } // namespace fiber::detail

#endif // ifndef fiber_detail_NullLockable_h
