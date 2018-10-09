#ifndef fiber_MultiThreadService_h
#define fiber_MultiThreadService_h

#include "Service.h"

#include "detail/StdTraits.h"

namespace fiber {

typedef Service<detail::StdTraits> StdService;

} // namespace fiber

#endif // ifndef fiber_MultiThreadService_h
