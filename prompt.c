#include "prompt.h"
#include "directory.h"

LPWSTR get_prompt() {
  LPWSTR cwd = get_current_directory();
  int len = wcslen(cwd);
  cwd = realloc(cwd,
                (len + 4) *
                    sizeof(WCHAR)); // Add space for " > " and null terminator
  cwd[len] = L'\n';
  cwd[len + 2] = L' ';
  cwd[len + 1] = L'>';
  cwd[len + 3] = L'\0';
  return cwd;
}
