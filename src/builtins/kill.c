#include "kill.h"

#include <stdlib.h>
#include <wchar.h>
#include <windows.h>

#include "../dispatcher.h"
#include "../process_manager.h"


static ExecutionResult handler(int argc, LPWSTR* argv) {
  if (argc != 2) {
    wprintf(L"Usage: kill <PID>\n");
    return KEEP_RUNNING(1);
  }

  DWORD target_pid = (DWORD)_wtoi(argv[1]);

  int index = find_process_index(target_pid);

  if (index == -1) {
    wprintf(L"Error: PID %lu not found in background processes.\n", target_pid);
    return KEEP_RUNNING(1);
  }

  if (TerminateProcess(bg_processes[index].handle, 0)) {
    wprintf(L"Process %lu terminated successfully.\n", target_pid);
    remove_background_process(target_pid);
  } else {
    wprintf(L"Failed to terminate process %lu. Error code: %lu\n", target_pid,
            GetLastError());
  }

  return KEEP_RUNNING(0);
}

Command kill_command = {
    .name = L"kill",
    .description = L"Terminates a background process by its PID",
    .handler = handler};