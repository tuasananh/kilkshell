#include <wchar.h>

#include "command.h"

static ExecutionResult handler(int argc, LPWSTR* argv) {
  wprintf(L"Available commands: \n");
  int max_command_name_length = 18;
  for (size_t i = 0; i < n_commands; i++) {
    int command_name_length = wcslen(commands[i]->name);
    if (command_name_length > max_command_name_length) {
      max_command_name_length = command_name_length;
    }
  }
  for (size_t i = 0; i < n_commands; i++) {
    wprintf(L"  %-*ls - %ls\n", max_command_name_length, commands[i]->name,
            commands[i]->description);
  }
  wprintf(L"  %-*ls - Launch a process\n", max_command_name_length,
          L"<proc> <args...>");
  wprintf(L"  %-*ls - Launch a process in the background",
          max_command_name_length, L"<proc> <args...> &");
  return KEEP_RUNNING(0);
}

Command help_command = {
    .name = L"help", .description = L"Shows help message", .handler = handler};
