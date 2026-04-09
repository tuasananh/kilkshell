#include <stdbool.h>
#include <windows.h>

typedef struct {
  bool keep_running;
  int exit_status;
} ExecutionResult;

ExecutionResult dispatch_command(int argc, LPWSTR *argv);
