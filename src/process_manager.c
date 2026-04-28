#include "process_manager.h"

#include <processthreadsapi.h>
#include <stdio.h>

#include "data_structures/vector.h"

static Vector bg_processes;

void process_manager_init(void) {
  vector_init(&bg_processes, sizeof(ProcessInfo));
}

void process_manager_cleanup(void) {
  for (size_t i = 0; i < vector_size(&bg_processes); i++) {
    ProcessInfo* p = (ProcessInfo*)vector_get(&bg_processes, i);
    TerminateProcess(p->handle, 1);
    wprintf(L"Terminated background process with PID %lu\n", p->pid);
    CloseHandle(p->handle);
  }
  vector_free(&bg_processes);
}

ProcessInfo* process_manager_get(size_t index) {
  return (ProcessInfo*)vector_get(&bg_processes, index);
}

size_t process_manager_count(void) { return vector_size(&bg_processes); }

void add_background_process(DWORD pid, HANDLE handle, LPCWSTR command) {
  ProcessInfo p;
  p.pid = pid;
  p.handle = handle;
  p.status = STATUS_RUNNING;
  wcsncpy(p.command, command, 255);
  p.command[255] = L'\0';

  if (!vector_push(&bg_processes, &p)) {
    wprintf(L"Warning: Could not add background process (out of memory).\n");
  }
}

void remove_background_process(DWORD pid) {
  int index = find_process_index(pid);
  if (index == -1) return;

  ProcessInfo* p = (ProcessInfo*)vector_get(&bg_processes, (size_t)index);
  CloseHandle(p->handle);
  vector_remove(&bg_processes, (size_t)index);
}

int find_process_index(DWORD pid) {
  for (size_t i = 0; i < vector_size(&bg_processes); i++) {
    ProcessInfo* p = (ProcessInfo*)vector_get(&bg_processes, i);
    if (p->pid == pid) {
      return (int)i;
    }
  }
  return -1;
}

void clean_dead_processes(void) {
  for (size_t i = 0; i < vector_size(&bg_processes);) {
    ProcessInfo* p = (ProcessInfo*)vector_get(&bg_processes, i);
    DWORD exit_code;

    if (GetExitCodeProcess(p->handle, &exit_code)) {
      if (exit_code != STILL_ACTIVE) {
        remove_background_process(p->pid);
        continue;
      }
    }
    i++;
  }
}