#include "command.h"

#include "cd.h"
#include "exit.h"
#include "help.h"
#include "ls.h"

Command* commands[] = {
    &help_command, &exit_command, &ls_command, &cd_command
    // Add more commands here
};

const size_t n_commands = sizeof(commands) / sizeof(commands[0]);
