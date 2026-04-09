#include "dispatcher.h"
#include <assert.h>
#include <wchar.h>

ExecutionResult dispatch_command(int argc, LPWSTR *argv) {
  assert(argc > 0);

  if (wcscmp(argv[0], L"exit") == 0) {
    return (ExecutionResult){.keep_running = false, .exit_status = 0};
  }

  wprintf(L"Command not recognized: %ls\n", argv[0]);

  return (ExecutionResult){.keep_running = true, .exit_status = 1};
}
