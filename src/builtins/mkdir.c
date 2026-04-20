#include "mkdir.h"

#include <wchar.h>
#include <winnt.h>

#include "../dispatcher.h"

static ExecutionResult wrong_usage(LPWSTR command) {
  wprintf(L"Usage: %ls <directory>\n", command);
  return KEEP_RUNNING(EXIT_FAILURE);
}

static ExecutionResult handler(int argc, LPWSTR* argv) {
  if (argc != 2) {
    return wrong_usage(argv[0]);
  }

  BOOL success = CreateDirectoryW(argv[1], NULL);

  if (!success) {
    DWORD error = GetLastError();

    if (error == ERROR_ALREADY_EXISTS) {
      return keep_running_with_error(argv[0], error,
                                     L"Directory already exists");
    } else if (error == ERROR_PATH_NOT_FOUND) {
      return keep_running_with_error(
          argv[0], error, L"One or more intermediate directories do not exist");
    } else {
      return keep_running_with_error(argv[0], error,
                                     L"Cannot create the directory");
    }
  }

  return KEEP_RUNNING(0);
}

Command mkdir_command = {
    .name = L"mkdir",
    .description = L"Create the directory if it does not exist",
    .handler = handler};
