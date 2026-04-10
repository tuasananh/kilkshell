#include "exit.h"

#include <wchar.h>

#include "dispatcher.h"

static ExecutionResult wrong_usage() {
  wprintf(L"Usage: exit [code]\n");
  return (ExecutionResult){.keep_running = true, .exit_code = 1};
}

static ExecutionResult handler(int argc, LPWSTR* argv) {
  int exit_status = 0;
  if (argc > 2) {
    return wrong_usage();
  }
  if (argc > 1) {
    if (swscanf_s(argv[1], L"%d", &exit_status) != 1) {
      return wrong_usage();
    }
  }
  return (ExecutionResult){.keep_running = false, .exit_code = exit_status};
}

Command exit_command = {
    .name = L"exit", .description = L"Exit shell", .handler = handler};
