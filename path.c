#include "path.h"

#include <stdlib.h>
#include <wchar.h>
#include <windows.h>

#include "dispatcher.h"

static ExecutionResult path_handler(int argc, LPWSTR* argv) {
  (void)argc;
  (void)argv;

  DWORD buffer_size = GetEnvironmentVariableW(L"PATH", NULL, 0);
  if (buffer_size == 0) {
    wprintf(L"PATH environment variable is empty or not found.\n");
    return KEEP_RUNNING(0);
  }

  LPWSTR buffer = (LPWSTR)malloc(buffer_size * sizeof(WCHAR));
  if (buffer == NULL) {
    wprintf(L"Error: Not enough memory!\n");
    return KEEP_RUNNING(EXIT_FAILURE);
  }

  GetEnvironmentVariableW(L"PATH", buffer, buffer_size);
  wprintf(L"Current PATH: %ls\n", buffer);
  free(buffer);

  return KEEP_RUNNING(0);
}

Command path_command = {
    .name = L"path",
    .description = L"Displays the current PATH environment variable",
    .handler = path_handler};

static ExecutionResult addpath_handler(int argc, LPWSTR* argv) {
  if (argc != 2) {
    wprintf(L"Usage: %ls <directory>\n", argv[0]);
    return KEEP_RUNNING(EXIT_FAILURE);
  }

  DWORD attribs = GetFileAttributesW(argv[1]);
  if (attribs == INVALID_FILE_ATTRIBUTES ||
      !(attribs & FILE_ATTRIBUTE_DIRECTORY)) {
    wprintf(L"Error: The directory '%ls' does not exist.\n", argv[1]);
    return KEEP_RUNNING(EXIT_FAILURE);
  }

  DWORD old_size = GetEnvironmentVariableW(L"PATH", NULL, 0);
  DWORD new_dir_len = wcslen(argv[1]);
  DWORD new_size = (old_size > 0 ? old_size : 1) + new_dir_len + 1;

  LPWSTR new_path = (LPWSTR)malloc(new_size * sizeof(WCHAR));
  if (new_path == NULL) {
    wprintf(L"Error: Not enough memory!\n");
    return KEEP_RUNNING(EXIT_FAILURE);
  }

  if (old_size > 0) {
    GetEnvironmentVariableW(L"PATH", new_path, old_size);

    LPWSTR found = wcsstr(new_path, argv[1]);
    if (found != NULL) {
      size_t len = wcslen(argv[1]);
      if ((found == new_path || found[-1] == L';') &&
          (found[len] == L';' || found[len] == L'\0')) {
        wprintf(L"Notice: The directory is already in PATH.\n");
        free(new_path);
        return KEEP_RUNNING(0);
      }
    }

    wcscat(new_path, L";");
    wcscat(new_path, argv[1]);
  } else {
    wcscpy(new_path, argv[1]);
  }

  if (!SetEnvironmentVariableW(L"PATH", new_path)) {
    ExecutionResult res = keep_running_with_error(argv[0], GetLastError(),
                                                  L"Failed to update PATH");
    free(new_path);
    return res;
  }

  free(new_path);
  wprintf(L"Success: Directory added to PATH.\n");
  return KEEP_RUNNING(0);
}

Command addpath_command = {
    .name = L"addpath",
    .description = L"Appends a new directory to the PATH variable",
    .handler = addpath_handler};
