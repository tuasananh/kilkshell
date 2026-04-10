#include "dispatcher.h"

#include <assert.h>
#include <wchar.h>

#include "command.h"

ExecutionResult dispatch_command(int argc, LPWSTR* argv) {
  assert(argc > 0);

  for (int i = 0; i < n_commands; i++) {
    if (wcscmp(argv[0], commands[i]->name) == 0) {
      return commands[i]->handler(argc, argv);
    }
  }

  wprintf(L"Command not recognized: %ls\n", argv[0]);
  return KEEP_RUNNING(EXIT_FAILURE);
}
