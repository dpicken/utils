#ifndef assert_verifyChildProcess_h
#define assert_verifyChildProcess_h

#include <cstdint>

namespace assert {

void verifyChildProcessExitsWithSuccess();

void verifyChildProcessExitsWithFailure();

void verifyChildProcessExitsWithStatus(std::uint8_t expectedStatus);

void verifyChildProcessAborts();

} // namespace assert

#endif // ifndef assert_verifyChildProcess_h
