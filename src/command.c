#include "command.h"

#include "builtins/cd.h"
#include "builtins/clear.h"
#include "builtins/date.h"
#include "builtins/envget.h"
#include "builtins/envset.h"
#include "builtins/exit.h"
#include "builtins/help.h"
#include "builtins/kill.h"
#include "builtins/list.h"
#include "builtins/ls.h"
#include "builtins/mkdir.h"
#include "builtins/path.h"
#include "builtins/resume.h"
#include "builtins/stop.h"
#include "builtins/time.h"

Command* commands[] = {
    &help_command,  &exit_command,    &ls_command,     &cd_command,
    &clear_command, &mkdir_command,   &date_command,   &time_command,
    &path_command,  &addpath_command, &list_command,   &kill_command,
    &stop_command,  &resume_command,  &envset_command, &envget_command};

const size_t n_commands = sizeof(commands) / sizeof(commands[0]);
