#include <windows.h>

/*
 * Get the current working directory as a wide string.
 * The caller takes ownership of the returned string.
 */
LPWSTR get_current_directory();
