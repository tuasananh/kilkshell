#include "command.h"

#include "cd.h"
#include "clear.h"
#include "date.h"
#include "exit.h"
#include "help.h"
#include "kill.h"
#include "list.h"
#include "ls.h"
#include "mkdir.h"
#include "path.h"
#include "resume.h"
#include "stop.h"
#include "time.h"

Command* commands[] = {&help_command,    &exit_command,  &ls_command,
                       &cd_command,      &clear_command, &mkdir_command,
                       &date_command,    &time_command,  &path_command,
                       &addpath_command, &list_command,  &kill_command,
                       &stop_command,    &resume_command};

const size_t n_commands = sizeof(commands) / sizeof(commands[0]);
