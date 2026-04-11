#include "cd.h"

#include <processenv.h>
#include <wchar.h>

#include "dispatcher.h"

static ExecutionResult wrong_usage(LPWSTR command) {
  wprintf(L"Usage: %ls <directory>\n", command);
  return KEEP_RUNNING(EXIT_FAILURE);
}

static ExecutionResult handler(int argc, LPWSTR* argv) {
  if (argc != 2) {
    return wrong_usage(argv[0]);
  }

  BOOL success = SetCurrentDirectoryW(argv[1]);

  if (!success) {
    return keep_running_with_error(argv[0], GetLastError(),
                                   L"Cannot change the current directory");
  }

  return KEEP_RUNNING(0);
}

Command cd_command = {
    .name = L"cd",
    .description = L"Change the shell working directory",
    .handler = handler,
};
