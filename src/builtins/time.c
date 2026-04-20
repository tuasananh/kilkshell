#include "time.h"

#include <wchar.h>
#include <windows.h>

#include "../dispatcher.h"

static ExecutionResult handler(int argc, LPWSTR* argv) {
  (void)argc;
  (void)argv;

  SYSTEMTIME st;

  GetLocalTime(&st);

  wprintf(L"Current time: %02d:%02d:%02d\n", st.wHour, st.wMinute, st.wSecond);

  return KEEP_RUNNING(0);
}

Command time_command = {.name = L"time",
                        .description = L"Displays the current system time",
                        .handler = handler};