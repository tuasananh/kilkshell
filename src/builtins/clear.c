#include "clear.h"

#include <synchapi.h>
#include <windows.h>

// Source - https://stackoverflow.com/a/42500322
static void clear_screen() {
  HANDLE hStdOut;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD count;
  DWORD cellCount;
  COORD homeCoords = {0, 0};

  hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  if (hStdOut == INVALID_HANDLE_VALUE) return;

  /* Get the number of cells in the current buffer */
  if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
  cellCount = csbi.dwSize.X * csbi.dwSize.Y;

  /* Fill the entire buffer with spaces */
  if (!FillConsoleOutputCharacter(hStdOut, (TCHAR)' ', cellCount, homeCoords,
                                  &count))
    return;

  /* Fill the entire buffer with the current colors and attributes */
  if (!FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, cellCount,
                                  homeCoords, &count))
    return;

  /* Move the cursor home */
  SetConsoleCursorPosition(hStdOut, homeCoords);
}

static ExecutionResult handler(int argc, LPWSTR* argv) {
  clear_screen();
  return KEEP_RUNNING(0);
}

Command clear_command = {.name = L"clear",
                         .description = L"Clears the console screen.",
                         .handler = handler};
