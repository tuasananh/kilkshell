#include "dispatcher.h"

#include <assert.h>
#include <processthreadsapi.h>
#include <wchar.h>

#include "command.h"

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

  LocalFree(argv);

  STARTUPINFOW si;
  PROCESS_INFORMATION pi;

  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));

  BOOL success = CreateProcessW(NULL, input_buffer, NULL, NULL, FALSE, 0, NULL,
                                NULL, &si, &pi);

  if (!success) {
    DWORD error_code = GetLastError();
    return keep_running_with_error(L"kilkshell", error_code,
                                   L"Command not found");
  }

  WaitForSingleObject(pi.hProcess, INFINITE);

  DWORD exit_code;
  GetExitCodeProcess(pi.hProcess, &exit_code);

  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);

  return KEEP_RUNNING(exit_code);
}

ExecutionResult keep_running_with_error(LPWSTR command, DWORD error_code,
                                        LPWSTR message) {
  if (message == NULL) {
    message = L"An unexpected error occurred";
  }
  wprintf(L"%ls: Error %zd: %ls.\n", command, error_code, message);
  return KEEP_RUNNING(EXIT_FAILURE);
}
