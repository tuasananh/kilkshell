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
  ((ExecutionResult){.keep_running = true, .exit_code = (code)})

ExecutionResult dispatch_command(int argc, LPWSTR* argv);
