#ifndef PSH_CMD_H__
#define PSH_CMD_H__

#include "fs/psh_fs.h"

#define PSH_HELP_CMD_NAME "help"
#define PSH_HELP_CMD_HELP "Display this help"
#define PSH_LS_CMD_NAME "ls"
#define PSH_LS_CMD_HELP "List pico-filesystem content"
#define PSH_CAT_CMD_NAME "cat"
#define PSH_CAT_CMD_HELP "List content of a readable file"
#define PSH_ECHO_CMD_NAME "echo"
#define PSH_ECHO_CMD_HELP "Echoes arguments to standard output"

int psh_add_util_cmds(void);

#endif
