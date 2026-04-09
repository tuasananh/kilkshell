#include <windows.h>

/*
 * Get the prompt string, which consists of the current working directory
 * followed by a newline and "> ".
 * The caller takes ownership of the returned string.
 */
LPWSTR get_prompt();
