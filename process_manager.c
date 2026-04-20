#include "process_manager.h"

#include <stdio.h>

ProcessInfo bg_processes[MAX_BG_PROCESSES];
int bg_process_count = 0;

void add_background_process(DWORD pid, HANDLE handle, LPCWSTR command) {
  if (bg_process_count >= MAX_BG_PROCESSES) {
    wprintf(L"Warning: Background process limit reached (%d).\n",
            MAX_BG_PROCESSES);
    return;
  }

  bg_processes[bg_process_count].pid = pid;
  bg_processes[bg_process_count].handle = handle;
  bg_processes[bg_process_count].status = STATUS_RUNNING;

  wcsncpy(bg_processes[bg_process_count].command, command, 255);
  bg_processes[bg_process_count].command[255] = L'\0';

  bg_process_count++;
}

void remove_background_process(DWORD pid) {
  int index = find_process_index(pid);
  if (index == -1) return;

  CloseHandle(bg_processes[index].handle);

  for (int i = index; i < bg_process_count - 1; i++) {
    bg_processes[i] = bg_processes[i + 1];
  }
  bg_process_count--;
}

int find_process_index(DWORD pid) {
  for (int i = 0; i < bg_process_count; i++) {
    if (bg_processes[i].pid == pid) {
      return i;
    }
  }
  return -1;
}

void clean_dead_processes() {
  for (int i = 0; i < bg_process_count;) {
    DWORD exit_code;

    if (GetExitCodeProcess(bg_processes[i].handle, &exit_code)) {
      if (exit_code != STILL_ACTIVE) {
        remove_background_process(bg_processes[i].pid);
        continue;
      }
    }
    i++;
  }
}