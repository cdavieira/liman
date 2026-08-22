#include "platform/unix/process.h"
#include "platform/abort.h"
#include "platform/log.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int process_dispatch_cmd(char **cmds) {
  int res = fork();

  if (res == -1) {
    abort_default("Fork failed");
    return -1; // make my editor happy
  }

  if (res == 0) {
    // for (int i = 0; cmds[i] != NULL; i++) {
    //   fprintf(stderr, "argv[%d] = [%s]\n", i, cmds[i]);
    // }

    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    execvp(cmds[0], cmds);
    abort_throw(127, "");
    return -1; // make my editor happy
  }

  int status = 0;

  int child_pid = wait(&status); // child_pid should be eq to res

  int exit_status = WEXITSTATUS(status);

  if (WIFEXITED(status)) {
    // log_info("Process %d exited with status %d", res, exit_status);

    return exit_status == 0 ? 0 : -1;
  }

  if (WIFSIGNALED(status)) {
    // log_warning("Process %d was killed by signal %d", res, exit_status);

    return 0;
  }

  return -1;
}
