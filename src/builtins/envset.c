#include "envset.h"

#include <stdlib.h>
#include <wchar.h>
#include <windows.h>

#include "../dispatcher.h"

static ExecutionResult wrong_usage(LPWSTR command) {
  wprintf(L"Usage: %ls <name> <value>\n", command);
  return KEEP_RUNNING(EXIT_FAILURE);
}

static ExecutionResult handler(int argc, LPWSTR* argv) {
  if (argc != 3) {
    return wrong_usage(argv[0]);
  }

  LPWSTR var_name = argv[1];
  LPWSTR var_value = argv[2];

  if (!SetEnvironmentVariableW(var_name, var_value)) {
    return keep_running_with_error(argv[0], GetLastError(),
                                   L"Failed to set environment variable");
  }

  return KEEP_RUNNING(0);
}

Command envset_command = {
    .name = L"envset",
    .description = L"Set an environment variable",
    .handler = handler,
};
