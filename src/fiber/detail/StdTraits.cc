#include "StdTraits.h"

void fiber::detail::stdFiberStartContext(void* arg) {
  reinterpret_cast<Fiber<StdTraits>*>(arg)->run();
}
