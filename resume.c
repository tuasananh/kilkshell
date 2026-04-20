#include "resume.h"

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <windows.h>

#include "dispatcher.h"
#include "process_manager.h"

typedef LONG(NTAPI* pNtResumeProcess)(HANDLE ProcessHandle);

static ExecutionResult resume_handler(int argc, LPWSTR* argv) {
  if (argc != 2) {
    wprintf(L"Usage: resume <PID>\n");
    return KEEP_RUNNING(1);
  }

  DWORD target_pid = (DWORD)_wtoi(argv[1]);
  int index = find_process_index(target_pid);

  if (index == -1) {
    wprintf(L"Error: PID %lu not found in background processes.\n", target_pid);
    return KEEP_RUNNING(1);
  }

  HMODULE hNtdll = GetModuleHandleW(L"ntdll.dll");
  if (!hNtdll) {
    wprintf(L"Error: Could not load ntdll.dll\n");
    return KEEP_RUNNING(1);
  }

  pNtResumeProcess NtResumeProcess =
      (pNtResumeProcess)(void*)GetProcAddress(hNtdll, "NtResumeProcess");

  if (NtResumeProcess) {
    LONG status = NtResumeProcess(bg_processes[index].handle);
    if (status >= 0) {
      bg_processes[index].status = STATUS_RUNNING;
      wprintf(L"Process %lu resumed.\n", target_pid);
    } else {
      wprintf(L"Failed to resume process %lu. NTSTATUS: 0x%lX\n", target_pid,
              status);
    }
  } else {
    wprintf(L"Error: Could not locate NtResumeProcess in ntdll.dll\n");
    return KEEP_RUNNING(1);
  }

  return KEEP_RUNNING(0);
}

// 4. Gói lệnh Resume
Command resume_command = {
    .name = L"resume",
    .description = L"Resume a suspended background process by its PID",
    .handler = resume_handler};