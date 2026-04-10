#include "exit.h"

#include <stdlib.h>
#include <wchar.h>

#include "dispatcher.h"

static ExecutionResult wrong_usage(LPWSTR command) {
  wprintf(L"Usage: %ls [code]\n", command);
  return KEEP_RUNNING(EXIT_FAILURE);
}

static ExecutionResult handler(int argc, LPWSTR* argv) {
  int exit_status = 0;
  if (argc > 2) {
    return wrong_usage(argv[0]);
  }
  if (argc > 1) {
    if (swscanf_s(argv[1], L"%d", &exit_status) != 1) {
      return wrong_usage(argv[0]);
    }
  }

  return END_SHELL(exit_status);
}

Command exit_command = {
    .name = L"exit", .description = L"Exit shell", .handler = handler};
