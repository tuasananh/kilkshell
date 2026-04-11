#pragma once
#include <windows.h>

#include "dispatcher.h"

ExecutionResult run_process(LPWSTR input_buffer, bool run_in_background);
