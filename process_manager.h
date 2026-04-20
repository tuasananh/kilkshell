#pragma once

#include <wchar.h>
#include <windows.h>

typedef enum { STATUS_RUNNING, STATUS_STOPPED } ProcessStatus;

typedef struct {
  DWORD pid;
  HANDLE handle;
  WCHAR command[256];
  ProcessStatus status;
} ProcessInfo;
#define MAX_BG_PROCESSES 100

extern ProcessInfo bg_processes[MAX_BG_PROCESSES];
extern int bg_process_count;

void add_background_process(DWORD pid, HANDLE handle, LPCWSTR command);
void remove_background_process(DWORD pid);
int find_process_index(DWORD pid);
void clean_dead_processes();