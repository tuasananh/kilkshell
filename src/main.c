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
  GetCurrentDirectoryW(length, prompt);
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

  int exit_code = 0;

  while (true) {
    LPWSTR prompt = get_prompt();
    wprintf(L"%ls", prompt);
    free(prompt);  // we are done

    HRESULT result = StringCchGetsW(input_buffer, MAX_INPUT_SIZE);

    if (FAILED(result)) {
      if (result == STRSAFE_E_INVALID_PARAMETER) {
        wprintf(L"Invalid parameter. Please try again.\n");
      } else if (result == STRSAFE_E_INSUFFICIENT_BUFFER) {
        wprintf(L"Input too long. Please limit to %d characters.\n",
                MAX_INPUT_SIZE - 1);
      } else if (result != STRSAFE_E_END_OF_FILE || !feof(stdin)) {
        wprintf(L"An unexpected error occurred.\n");
      }
    } else {
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
    }

    wprintf(L"\n");
  }

  return exit_code;
}
