#ifndef context_Context_h
#define context_Context_h

#include <cstddef>

extern "C" {

namespace context {

/** Context for a thread of execution. */
struct Context {
  void* sp;
};

/** Entry point for a thread of execution. */
typedef void (*StartContextFn)(void* arg);

/**
 * Initializes a context for a new thread of execution.
 *
 * Switching to the returned context will result in a call to startContextFn(arg).
 *
 * N.B. If startContextFn returns, then exit(0) is called.
 */
Context initContext(void* stack, std::size_t stackSize, StartContextFn startContextFn, void* arg);

/**
 * Performs a context switch.
 *
 * Stores the state of the current context in fromContext then switches the current thread of execution to toContext.
 */
void switchContext(Context* fromContext, Context toContext);

} // namespace context

} // extern "C"

#endif // ifndef context_Context_h
