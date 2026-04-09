#include "dispatcher.h"
#include "prompt.h"
#include <fcntl.h>
#include <io.h>
#include <stdbool.h>
#include <stdio.h>
#include <wchar.h>
#include <windows.h>

#define MAX_INPUT_SIZE 1024

WCHAR input_buffer[MAX_INPUT_SIZE];

int main() {
  _setmode(_fileno(stdout), _O_U16TEXT); // Set stdout to Unicode mode

  LPWSTR prompt = get_prompt();

  while (true) {
    wprintf(L"%ls", prompt);

    if (fgetws(input_buffer, MAX_INPUT_SIZE, stdin) == NULL) {
      wprintf(L"Error reading input. Ignoring...\n");
      continue;
    }

    size_t input_len = wcslen(input_buffer);

    if (input_len != 1 || input_buffer[0] != L'\n') { // User just pressed Enter
      int argc;
      LPWSTR *argv = CommandLineToArgvW(input_buffer, &argc);

      ExecutionResult result = dispatch_command(argc, argv);
      LocalFree(argv);

      if (!result.keep_running) {
        break;
      }
    }

    wprintf(L"\n");
  }

  free(prompt);
}
