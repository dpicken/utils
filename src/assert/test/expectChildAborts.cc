#include "expectChildAborts.h"

#include "assert/Assert.h"

#include <csignal>
#include <sys/wait.h>

void assert::test::expectChildAborts() {
    int status = 0;
    VERIFY(wait(&status) != -1);
    VERIFY(WIFSIGNALED(status));
    VERIFY(WTERMSIG(status) == SIGABRT);
}
