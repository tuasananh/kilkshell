#include "stop.h"

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <windows.h>

#include "../dispatcher.h"
#include "../process_manager.h"

typedef LONG(NTAPI* pNtSuspendProcess)(HANDLE ProcessHandle);

static ExecutionResult stop_handler(int argc, LPWSTR* argv) {
  if (argc != 2) {
    wprintf(L"Usage: stop <PID>\n");
    return KEEP_RUNNING(1);
  }

  DWORD target_pid = (DWORD)_wtoi(argv[1]);
  int index = find_process_index(target_pid);

  if (index == -1) {
    wprintf(L"Error: PID %lu not found in background processes.\n", target_pid);
    return KEEP_RUNNING(1);
  }

  HMODULE hNtdll = GetModuleHandleW(L"ntdll.dll");
  if (!hNtdll) return KEEP_RUNNING(1);

  pNtSuspendProcess NtSuspendProcess =
      (pNtSuspendProcess)(void*)GetProcAddress(hNtdll, "NtSuspendProcess");

  if (NtSuspendProcess) {
    LONG status = NtSuspendProcess(bg_processes[index].handle);
    if (status >= 0) {
      bg_processes[index].status = STATUS_STOPPED;
      wprintf(L"Process %lu suspended.\n", target_pid);
    } else {
      wprintf(L"Failed to suspend process %lu. NTSTATUS: 0x%lX\n", target_pid,
              status);
    }
  }

  return KEEP_RUNNING(0);
}

Command stop_command = {
    .name = L"stop",
    .description = L"Suspend a background process by its PID",
    .handler = stop_handler};