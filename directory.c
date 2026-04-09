#include "directory.h"
#include <wchar.h>

LPWSTR get_current_directory() {
  DWORD length = GetCurrentDirectoryW(0, NULL);
  LPWSTR buffer = (LPWSTR)malloc(length * sizeof(WCHAR));
  length = GetCurrentDirectoryW(MAX_PATH, buffer);
  if (length == 0) {
    wprintf(L"Failed to get current directory. Error code: %lu\n",
            GetLastError());
    exit(EXIT_FAILURE);
  }
  return buffer;
}
