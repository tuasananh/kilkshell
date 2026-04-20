#pragma once
#include <stdbool.h>
#include <windows.h>

typedef struct ExecutionResult {
  bool keep_running;
  int exit_code;
} ExecutionResult;

#define KEEP_RUNNING(code) \
  ((ExecutionResult){.keep_running = true, .exit_code = (code)})
#define END_SHELL(code) \
  ((ExecutionResult){.keep_running = false, .exit_code = (code)})

ExecutionResult keep_running_with_error(LPWSTR command, DWORD error_code,
                                        LPWSTR message);

ExecutionResult dispatch_command(LPWSTR input_buffer, DWORD input_len);
