#include "help.h"

#include <wchar.h>

#include "command.h"

static ExecutionResult handler(int argc, LPWSTR* argv) {
  wprintf(L"Available commands: \n");
  for (size_t i = 0; i < n_commands; i++) {
    wprintf(L"  %ls - %ls\n", commands[i]->name, commands[i]->description);
  }
  return (ExecutionResult){.keep_running = true, .exit_code = 0};
}

Command help_command = {
    .name = L"help", .description = L"Shows help message", .handler = handler};
