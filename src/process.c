#include "process.h"

#include <handleapi.h>
#include <stdint.h>
#include <wchar.h>

#include "process_manager.h"

// Encodes the input string and wraps it in the PowerShell command directly.
// Returns a single allocated string. Caller must call free() on it.
static LPWSTR wrap_in_powershell_encoded_command(LPCWSTR input_buffer) {
  if (input_buffer == NULL) return NULL;

  static const wchar_t prefix[] = L"powershell -EncodedCommand ";
  size_t prefix_len = wcslen(prefix);  // Length is 27

  static const wchar_t b64_table[] =
      L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  const unsigned char* bytes = (const unsigned char*)input_buffer;
  size_t in_len = wcslen(input_buffer) * sizeof(wchar_t);  // Raw byte length

  // Calculate Base64 length: 4 chars for every 3 bytes, rounded up
  size_t b64_len = 4 * ((in_len + 2) / 3);

  // Total characters: Prefix + Base64 + 1 (for the null terminator)
  size_t total_chars = prefix_len + b64_len + 1;

  // Perform the single allocation
  LPWSTR final_command = (LPWSTR)malloc(total_chars * sizeof(wchar_t));
  if (final_command == NULL) return NULL;

  // 1. Copy the prefix to the start of the buffer
  wmemcpy(final_command, prefix, prefix_len);

  // 2. Encode Base64 directly into the buffer, starting right after the prefix
  size_t i = 0;
  size_t j = prefix_len;

  while (i < in_len) {
    uint32_t octet_a = i < in_len ? bytes[i++] : 0;
    uint32_t octet_b = i < in_len ? bytes[i++] : 0;
    uint32_t octet_c = i < in_len ? bytes[i++] : 0;

    uint32_t triple = (octet_a << 16) | (octet_b << 8) | octet_c;

    final_command[j++] = b64_table[(triple >> 18) & 0x3F];
    final_command[j++] = b64_table[(triple >> 12) & 0x3F];
    final_command[j++] = b64_table[(triple >> 6) & 0x3F];
    final_command[j++] = b64_table[triple & 0x3F];
  }

  // 3. Apply the '=' padding if needed
  // The null terminator will be at index (total_chars - 1)
  if (in_len % 3 == 1) {
    final_command[total_chars - 3] = L'=';
    final_command[total_chars - 2] = L'=';
  } else if (in_len % 3 == 2) {
    final_command[total_chars - 2] = L'=';
  }

  // 4. Null terminate the string
  final_command[total_chars - 1] = L'\0';

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
    if (error_code == ERROR_FILE_NOT_FOUND) {
      LPWSTR ps_cmd = wrap_in_powershell_encoded_command(input_buffer);
      if (ps_cmd != NULL) {
        success = CreateProcessW(NULL, ps_cmd, NULL, NULL, inherit_handles,
                                 creation_flags, NULL, NULL, &si, &pi);
        free(ps_cmd);
      } else {
        if (hNullIn != INVALID_HANDLE_VALUE) {
          CloseHandle(hNullIn);
        }
        return keep_running_with_error(
            L"kilkshell", error_code,
            L"Failed to allocate memory for PowerShell command");
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
    wprintf(L"[%d] Process started with PID: %d\n", bg_process_count,
            pi.dwProcessId);
  }

  if (pi.hProcess != NULL) CloseHandle(pi.hProcess);
  if (pi.hThread != NULL) CloseHandle(pi.hThread);

  if (hNullIn != INVALID_HANDLE_VALUE) {
    CloseHandle(hNullIn);
  }

  return KEEP_RUNNING(exit_code);
}
