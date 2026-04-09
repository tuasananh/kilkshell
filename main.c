#include "prompt.h"
#include <fcntl.h>
#include <io.h>
#include <stdbool.h>
#include <stdio.h>
#include <windows.h>

#define MAX_INPUT_SIZE 1024

WCHAR input_buffer[MAX_INPUT_SIZE];

int main() {
  _setmode(_fileno(stdout), _O_U16TEXT); // Set stdout to Unicode mode

  LPWSTR prompt = get_prompt();

  while (true) {
    wprintf(L"%ls", prompt);

    if (fgetws(input_buffer, MAX_INPUT_SIZE, stdin) == NULL) {
      wprintf(L"Error reading input. Exiting.\n");
      break;
    }

    wprintf(L"Input command: %ls", input_buffer);
  }

  free(prompt);
}
