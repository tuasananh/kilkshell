#include <fcntl.h>
#include <io.h>
#include <stdbool.h>
#include <stdio.h>
#include <wchar.h>
#include <windows.h>

#include "dispatcher.h"

#define MAX_INPUT_SIZE 1024

WCHAR input_buffer[MAX_INPUT_SIZE];

LPWSTR get_prompt() {
  DWORD length = GetCurrentDirectoryW(0, NULL);
  LPWSTR prompt = (LPWSTR)malloc((length + 3) * sizeof(WCHAR));
  GetCurrentDirectoryW(MAX_PATH, prompt);
  prompt[length - 1] = L'\n';
  prompt[length] = L'>';
  prompt[length + 1] = L' ';
  prompt[length + 2] = L'\0';
  return prompt;
}

int main() {
  _setmode(_fileno(stdout), _O_U16TEXT);  // Set stdout to Unicode mode

  LPWSTR prompt = get_prompt();

  int exit_code = 0;

  while (true) {
    wprintf(L"%ls", prompt);

    if (fgetws(input_buffer, MAX_INPUT_SIZE, stdin) == NULL) {
      wprintf(L"Error reading input. Ignoring...\n");
      continue;
    }

    size_t input_len = wcslen(input_buffer);

    // If user did not just press Enter
    if (input_len != 1 || input_buffer[0] != L'\n') {
      int argc;
      LPWSTR* argv = CommandLineToArgvW(input_buffer, &argc);

      ExecutionResult result = dispatch_command(argc, argv);
      LocalFree(argv);

      if (!result.keep_running) {
        exit_code = result.exit_code;
        break;
      }
    }

    wprintf(L"\n");
  }

  free(prompt);

  return exit_code;
}
