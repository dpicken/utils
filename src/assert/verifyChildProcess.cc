#include "verifyChildProcess.h"

#include "Assert.h"

#include <csignal>
#include <sys/wait.h>

void assert::verifyChildProcessExitsWithSuccess() {
    int status = 0;
    VERIFY(wait(&status) != -1);
    VERIFY(WIFEXITED(status));
    VERIFY_EQ(WEXITSTATUS(status), 0);
}

void assert::verifyChildProcessExitsWithFailure() {
    int status = 0;
    VERIFY(wait(&status) != -1);
    VERIFY(WIFEXITED(status));
    VERIFY_NE(WEXITSTATUS(status), 0);
}

void assert::verifyChildProcessExitsWithStatus(std::uint8_t expectedStatus) {
    int status = 0;
    VERIFY(wait(&status) != -1);
    VERIFY(WIFEXITED(status));
    VERIFY_EQ(WEXITSTATUS(status), expectedStatus);
}

void assert::verifyChildProcessAborts() {
    int status = 0;
    VERIFY(wait(&status) != -1);
    VERIFY(WIFSIGNALED(status));
    VERIFY_EQ(WTERMSIG(status), SIGABRT);
}
