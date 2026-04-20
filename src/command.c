#include "command.h"

#include "builtins/cd.h"
#include "builtins/clear.h"
#include "builtins/exit.h"
#include "builtins/help.h"
#include "builtins/ls.h"
#include "builtins/mkdir.h"

Command* commands[] = {
    &help_command,  &exit_command, &ls_command, &cd_command,
    &clear_command, &mkdir_command
    // Add more commands here
};

const size_t n_commands = sizeof(commands) / sizeof(commands[0]);
