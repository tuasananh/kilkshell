#include "date.h"

#include <wchar.h>
#include <windows.h>

#include "dispatcher.h"

static ExecutionResult handler(int argc, LPWSTR* argv) {
  (void)argc;
  (void)argv;
  SYSTEMTIME st;

  GetLocalTime(&st);

  wprintf(L"Current date: %02d/%02d/%04d\n", st.wDay, st.wMonth, st.wYear);

  return KEEP_RUNNING(0);
}

Command date_command = {.name = L"date",
                        .description = L"Displays the current system date",
                        .handler = handler};