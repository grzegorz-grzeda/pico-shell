#ifndef PSH_CLI_H__
#define PSH_CLI_H__

#include "utils/psh_errors.h"

typedef struct psh_cli_t
{
    struct
    {
        char *buffer;
        int size;
        int _cnt;
    } input;
    struct
    {
        char **buffer;
        int size;
        int _cnt;
    } params;
    const char *ps1;
    char delimiter;
    char new_line;
} psh_cli;

int psh_init_cli(psh_cli *cli);
int psh_print(const char *text);
int psh_process(void);

#endif
