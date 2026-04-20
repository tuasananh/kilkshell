#include "dispatcher.h"

#include <assert.h>
#include <processthreadsapi.h>
#include <stdint.h>
#include <wchar.h>
#include <winuser.h>

#include "command.h"
#include "process.h"

ExecutionResult dispatch_command(LPWSTR input_buffer, DWORD input_len) {
  int argc;
  LPWSTR* argv = CommandLineToArgvW(input_buffer, &argc);
  assert(argc > 0);

  for (size_t i = 0; i < n_commands; i++) {
    if (wcscmp(argv[0], commands[i]->name) == 0) {
      ExecutionResult res = commands[i]->handler(argc, argv);
      LocalFree(argv);
      return res;
    }
  }

  bool run_in_background = false;
  if (wcscmp(argv[argc - 1], L"&") == 0) {
    run_in_background = true;
  }
  LocalFree(argv);

  if (run_in_background) {
    while (input_len > 0 && input_buffer[input_len - 1] != L'&') {
      input_buffer[--input_len] = L'\0';
    }
    input_buffer[--input_len] = L'\0';  // removed the '&'
  }

  return run_process(input_buffer, run_in_background);
}

ExecutionResult keep_running_with_error(LPWSTR command, DWORD error_code,
                                        LPWSTR message) {
  if (message == NULL) {
    message = L"An unexpected error occurred";
  }
  wprintf(L"%ls: Error %zd: %ls.\n", command, error_code, message);
  return KEEP_RUNNING(EXIT_FAILURE);
}
