#pragma once

#include <windows.h>

#include "dispatcher.h"

typedef struct {
  LPCWSTR name;
  LPCWSTR description;
  ExecutionResult (*handler)(int argc, LPWSTR* argv);
} Command;

extern Command* commands[];

extern const size_t n_commands;
