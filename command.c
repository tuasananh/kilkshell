#include "command.h"

#include "cd.h"
#include "clear.h"
#include "exit.h"
#include "help.h"
#include "ls.h"
#include "mkdir.h"

Command* commands[] = {
    &help_command,  &exit_command, &ls_command, &cd_command,
    &clear_command, &mkdir_command
    // Add more commands here
};

const size_t n_commands = sizeof(commands) / sizeof(commands[0]);
