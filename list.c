#include "list.h"

#include <stdio.h>
#include <wchar.h>
#include <windows.h>

#include "dispatcher.h"
#include "process_manager.h"

ExecutionResult list_handler(int argc, LPWSTR* argv) {
  (void)argc;
  (void)argv;

  clean_dead_processes();

  if (bg_process_count == 0) {
    wprintf(L"No background processes running.\n");
    return KEEP_RUNNING(0);
  }

  wprintf(L"%-10s %-25s %-15s\n", L"PID", L"Command", L"Status");
  wprintf(L"--------------------------------------------------\n");

  for (int i = 0; i < bg_process_count; i++) {
    wprintf(L"%-10lu %-25ls %-15ls\n", bg_processes[i].pid,
            bg_processes[i].command,
            bg_processes[i].status == STATUS_RUNNING ? L"Running" : L"Stopped");
  }

  return KEEP_RUNNING(0);
}

Command list_command = {.name = L"list",
                        .description = L"Lists all background processes",
                        .handler = list_handler};