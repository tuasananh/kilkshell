#include <assert.h>
#include <fcntl.h>
#include <io.h>
#include <stdbool.h>
#include <stdio.h>
#include <strsafe.h>
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

    HRESULT result = StringCchGetsW(input_buffer, MAX_INPUT_SIZE);

    if (FAILED(result)) {
      wprintf(L"The command was too long. Please try again.\n");
    }

    size_t input_len;
    result = StringCchLengthW(input_buffer, MAX_INPUT_SIZE, &input_len);
    assert(SUCCEEDED(result));

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

  free(prompt);

  return exit_code;
}
