#include "ls.h"

#include <errhandlingapi.h>
#include <fileapi.h>
#include <handleapi.h>
#include <strsafe.h>
#include <wchar.h>
#include <windows.h>
#include <winerror.h>
#include <winnt.h>

#include "../dispatcher.h"

static ExecutionResult handler(int argc, LPWSTR* argv) {
  WCHAR directory[MAX_PATH];

  if (argc == 1) {
    directory[0] = L'.';
    directory[1] = L'\0';
  } else {
    LPWSTR end_ptr;
    if (FAILED(StringCchCopyExW(directory, MAX_PATH, argv[1], &end_ptr, NULL,
                                0))) {
      return keep_running_with_error(argv[0], GetLastError(),
                                     L"Directory path is too long");
    }
  }

  if (FAILED(StringCchCatW(directory, MAX_PATH, L"\\*"))) {
    return keep_running_with_error(argv[0], GetLastError(),
                                   L"Directory path is too long");
  }

  WIN32_FIND_DATAW ffd;
  HANDLE result = FindFirstFileW(directory, &ffd);

  if (result == INVALID_HANDLE_VALUE) {
    return keep_running_with_error(argv[0], GetLastError(),
                                   L"Cannot access the specified directory");
  }

  do {
    if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      wprintf(L"%ls/\n", ffd.cFileName);
    } else {
      wprintf(L"%ls\n", ffd.cFileName);
    }
  } while (FindNextFileW(result, &ffd));

  DWORD last_error = GetLastError();

  FindClose(result);
  if (last_error != ERROR_NO_MORE_FILES) {
    return keep_running_with_error(argv[0], last_error, NULL);
  }

  return KEEP_RUNNING(0);
}

Command ls_command = {.name = L"ls",
                      .description = L"List directory contents",
                      .handler = handler};
