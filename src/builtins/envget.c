#include "envget.h"

#include <stdlib.h>
#include <wchar.h>
#include <windows.h>

#include "../dispatcher.h"

static ExecutionResult wrong_usage(LPWSTR command) {
  wprintf(L"Usage: %ls <name>\n", command);
  return KEEP_RUNNING(EXIT_FAILURE);
}

static ExecutionResult handler(int argc, LPWSTR* argv) {
  if (argc != 2) {
    return wrong_usage(argv[0]);
  }

  LPWSTR var_name = argv[1];

  DWORD buffer_size = GetEnvironmentVariableW(var_name, NULL, 0);
  if (buffer_size == 0) {
    DWORD error = GetLastError();
    if (error == ERROR_ENVVAR_NOT_FOUND) {
      wprintf(L"Error: Environment variable '%ls' not found\n", var_name);
    } else {
      wprintf(L"Error: Failed to get environment variable\n");
    }
    return KEEP_RUNNING(EXIT_FAILURE);
  }

  LPWSTR buffer = (LPWSTR)malloc(buffer_size * sizeof(WCHAR));
  if (buffer == NULL) {
    wprintf(L"Error: Not enough memory!\n");
    return KEEP_RUNNING(EXIT_FAILURE);
  }

  GetEnvironmentVariableW(var_name, buffer, buffer_size);
  wprintf(L"%ls\n", buffer);
  free(buffer);

  return KEEP_RUNNING(0);
}

Command envget_command = {
    .name = L"envget",
    .description = L"Get an environment variable",
    .handler = handler,
};
