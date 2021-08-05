#include "cli/psh_cli.h"

static psh_node help_cmd;
static psh_node ls_cmd;
static psh_node cat_cmd;

int help_exe(struct psh_process_t *process, int argc, char **argv)
{
    psh_print(process->stdout, "Command list:\n");
    for (psh_node *node = process->root; node != 0; node = node->_next)
    {
        if (!node->execute)
        {
            continue;
        }
        psh_print(process->stdout, "  ");
        psh_print(process->stdout, node->name);
        psh_print(process->stdout, " - ");
        psh_print(process->stdout, node->help);
        psh_print(process->stdout, "\n");
    }
    return 0;
}

int ls_exe(struct psh_process_t *process, int argc, char **argv)
{
    psh_print(process->stdout, "Filesystem content:\n");
    for (psh_node *node = process->root; node != 0; node = node->_next)
    {
        char mod[] = "---";
        mod[0] = node->read ? 'r' : '-';
        mod[1] = node->write ? 'w' : '-';
        mod[2] = node->execute ? 'x' : '-';
        psh_print(process->stdout, "  ");
        psh_print(process->stdout, mod);
        psh_print(process->stdout, " ");
        psh_print(process->stdout, node->name);

        psh_print(process->stdout, "\n");
    }
    return 0;
}

int cat_exe(struct psh_process_t *process, int argc, char **argv)
{
    if (argc > 1)
    {
        psh_file *f = psh_open(process->root, argv[1]);

        if (f)
        {
            char c;
            while (psh_read(f, &c, 1) > 0)
            {
                psh_write(process->stdout, &c, 1);
            }
            psh_close(f);
        }
        else
        {
            psh_print(process->stderr, "Can't find file '");
            psh_print(process->stderr, argv[1]);
            psh_print(process->stderr, "'");
        }
    }
    psh_print(process->stderr, "\n");
    return 0;
}

int psh_cli_add_util_cmds(psh_context *ctx)
{
    help_cmd.name = "help";
    help_cmd.help = "Display this help";
    help_cmd.execute = help_exe;
    psh_mound_file(ctx, &help_cmd);

    ls_cmd.name = "ls";
    ls_cmd.help = "List pico-filesystem content";
    ls_cmd.execute = ls_exe;
    psh_mound_file(ctx, &ls_cmd);

    cat_cmd.name = "cat";
    cat_cmd.help = "List content of a readable file";
    cat_cmd.execute = cat_exe;
    psh_mound_file(ctx, &cat_cmd);
}
