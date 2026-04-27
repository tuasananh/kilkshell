#include <assert.h>
#include <fcntl.h>
#include <io.h>
#include <stdbool.h>
#include <stdio.h>
#include <wchar.h>
#include <windows.h>

#include "dispatcher.h"
#include "process_manager.h"

#define MAX_INPUT_SIZE 1024

WCHAR input_buffer[MAX_INPUT_SIZE];

LPWSTR get_prompt() {
  DWORD length = GetCurrentDirectoryW(0, NULL);
  LPWSTR prompt = (LPWSTR)malloc((length + 3) * sizeof(WCHAR));
  GetCurrentDirectoryW(length + 3, prompt);
  prompt[length - 1] = L'\n';
  prompt[length] = L'>';
  prompt[length + 1] = L' ';
  prompt[length + 2] = L'\0';
  return prompt;
}

BOOL WINAPI console_ctrl_handler(DWORD ctrl_type) {
  switch (ctrl_type) {
    case CTRL_C_EVENT:
      return TRUE;
    default:
      return FALSE;
  }
}

int main() {
  _setmode(_fileno(stdout), _O_U16TEXT);  // Set stdout to Unicode mode
  SetConsoleCtrlHandler(console_ctrl_handler, TRUE);
  process_manager_init();

  int exit_code = 0;

  while (true) {
    clean_dead_processes();
    LPWSTR prompt = get_prompt();
    wprintf(L"%ls", prompt);
    free(prompt);  // we are done

    if (fgetws(input_buffer, MAX_INPUT_SIZE, stdin) == NULL) {
      if (feof(stdin)) {
        break;
      }
      wprintf(L"An unexpected error occurred.\n");
      continue;
    }

    size_t input_len = wcslen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == L'\n') {
      input_buffer[input_len - 1] = L'\0';
      input_len--;
    } else if (input_len == MAX_INPUT_SIZE - 1) {
      wprintf(L"Input too long. Please limit to %d characters.\n",
              MAX_INPUT_SIZE - 2);
      // Flush stdin
      int c;
      while ((c = fgetwc(stdin)) != L'\n' && c != WEOF);
      continue;
    }

    // If user did not just press Enter
    if (input_len != 0) {
      ExecutionResult result = dispatch_command(input_buffer, input_len);

      if (!result.keep_running) {
        exit_code = result.exit_code;
        break;
      }
    }

    wprintf(L"\n");
  }

  process_manager_cleanup();
  return exit_code;
}
