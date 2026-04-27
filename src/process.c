#include "process.h"

#include <handleapi.h>
#include <stdint.h>
#include <wchar.h>

#include "process_manager.h"

// Wraps the input for execution via cmd.exe to support .bat files.
// Returns a single allocated string. Caller must call free() on it.
static LPWSTR wrap_in_cmd(LPCWSTR input_buffer) {
  if (input_buffer == NULL) return NULL;

  static const wchar_t prefix[] = L"cmd.exe /c \"";
  static const wchar_t suffix[] = L"\"";
  size_t prefix_len = wcslen(prefix); // Length is 12
  size_t suffix_len = wcslen(suffix); // Length is 1

  size_t in_len = wcslen(input_buffer);
  size_t total_chars = prefix_len + in_len + suffix_len + 1;

  LPWSTR final_command = (LPWSTR)malloc(total_chars * sizeof(wchar_t));
  if (final_command == NULL) return NULL;

  wcscpy(final_command, prefix);
  wcscat(final_command, input_buffer);
  wcscat(final_command, suffix);

  return final_command;
}

ExecutionResult run_process(LPWSTR input_buffer, bool run_in_background) {
  size_t len = wcslen(input_buffer);
  while (len > 0 &&
         (input_buffer[len - 1] == L'\n' || input_buffer[len - 1] == L'\r' ||
          input_buffer[len - 1] == L' ')) {
    input_buffer[--len] = L'\0';
  }

  if (len == 0) return KEEP_RUNNING(0);

  STARTUPINFOW si;
  PROCESS_INFORMATION pi;

  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));

  DWORD creation_flags = 0;
  BOOL inherit_handles = FALSE;
  HANDLE hNullIn = INVALID_HANDLE_VALUE;

  if (run_in_background) {
    inherit_handles = TRUE;

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    hNullIn = CreateFileW(L"NUL", GENERIC_READ, 0, &sa, OPEN_EXISTING,
                          FILE_ATTRIBUTE_NORMAL, NULL);
    if (hNullIn == INVALID_HANDLE_VALUE) {
      DWORD error_code = GetLastError();
      return keep_running_with_error(L"kilkshell", error_code,
                                     L"Failed to open NUL device");
    }

    si.hStdInput = hNullIn;
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    si.dwFlags |= STARTF_USESTDHANDLES;
  }

  BOOL success = CreateProcessW(NULL, input_buffer, NULL, NULL, inherit_handles,
                                creation_flags, NULL, NULL, &si, &pi);

  if (!success) {
    DWORD error_code = GetLastError();
    if (error_code == ERROR_FILE_NOT_FOUND || error_code == ERROR_BAD_EXE_FORMAT) {
      LPWSTR cmd_cmd = wrap_in_cmd(input_buffer);
      if (cmd_cmd != NULL) {
        success = CreateProcessW(NULL, cmd_cmd, NULL, NULL, inherit_handles,
                                 creation_flags, NULL, NULL, &si, &pi);
        free(cmd_cmd);
      } else {
        if (hNullIn != INVALID_HANDLE_VALUE) {
          CloseHandle(hNullIn);
        }
        return keep_running_with_error(
            L"kilkshell", error_code,
            L"Failed to allocate memory for cmd command");
      }

      if (!success) {
        error_code = GetLastError();
      }
    }

    if (!success) {
      if (hNullIn != INVALID_HANDLE_VALUE) {
        CloseHandle(hNullIn);
      }
      return keep_running_with_error(L"kilkshell", error_code,
                                     L"Command not found");
    }
  }

  DWORD exit_code = EXIT_SUCCESS;

  if (!run_in_background) {
    WaitForSingleObject(pi.hProcess, INFINITE);
    GetExitCodeProcess(pi.hProcess, &exit_code);
    CloseHandle(pi.hProcess);
  } else {
    add_background_process(pi.dwProcessId, pi.hProcess, input_buffer);
    wprintf(L"[%zu] Process started with PID: %d\n", process_manager_count(),
            pi.dwProcessId);
  }

  if (pi.hThread != NULL) CloseHandle(pi.hThread);

  if (hNullIn != INVALID_HANDLE_VALUE) {
    CloseHandle(hNullIn);
  }

  return KEEP_RUNNING(exit_code);
}
