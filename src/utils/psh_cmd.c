#include "utils/psh_cmd.h"
#include "utils/psh_utils.h"
#include "cli/psh_cli.h"

int help_exe(struct psh_file_frame_t *frame, int argc, char **argv)
{
    psh_print("Command list:\n");
    for (psh_file *f = frame->root; f != 0; f = f->_next)
    {
        if (!f->driver.execute)
        {
            continue;
        }
        psh_print("  ");
        psh_print(f->name);
        psh_print(" - ");
        psh_print(f->help);
        psh_print("\n");
    }
    return 0;
}

int ls_exe(struct psh_file_frame_t *frame, int argc, char **argv)
{
    psh_print("Filesystem content:\n");
    for (psh_file *f = frame->root; f != 0; f = f->_next)
    {
        if (f->_file.is_special)
        {
            continue;
        }

        char mod[] = "---";
        mod[0] = f->driver.read ? 'r' : '-';
        mod[1] = f->driver.write ? 'w' : '-';
        mod[2] = f->driver.execute ? 'x' : '-';
        psh_print("  ");
        psh_print(mod);
        psh_print(" ");
        psh_print(f->name);

        psh_print("\n");
    }
    return 0;
}

int cat_exe(struct psh_file_frame_t *process, int argc, char **argv)
{
    if (argc > 1)
    {
        psh_file *f = psh_open(argv[1]);

        if (f)
        {
            char c;
            while (psh_read(f, &c, 1) > 0)
            {
                psh_write(process->out, &c, 1);
            }
            psh_close(f);
        }
        else
        {
            psh_print("Can't find file '");
            psh_print(argv[1]);
            psh_print("'");
        }
    }
    psh_print("\n");
    return 0;
}
int echo_exe(struct psh_file_frame_t *process, int argc, char **argv)
{
    if (argc < 3)
    {
        return 0;
    }

    psh_file *f = psh_open(argv[1]);

    if (f)
    {
        psh_seek(f, 0, PSH_SEEK_SET);
        for (int i = 2; i < argc; i++)
        {
            psh_write(f, argv[i], string_length(argv[i]));
            if (i < (argc - 1))
            {
                psh_write(f, " ", 1);
            }
        }

        psh_close(f);
    }

    return 0;
}

int psh_add_util_cmds(void)
{
    psh_file *help = psh_mount_file(PSH_HELP_CMD_NAME, PSH_HELP_CMD_HELP);
    psh_file *ls = psh_mount_file(PSH_LS_CMD_NAME, PSH_LS_CMD_HELP);
    psh_file *cat = psh_mount_file(PSH_CAT_CMD_NAME, PSH_CAT_CMD_HELP);
    psh_file *echo = psh_mount_file(PSH_ECHO_CMD_NAME, PSH_ECHO_CMD_HELP);

    if (help && ls && cat && echo)
    {
        help->driver.execute = help_exe;
        ls->driver.execute = ls_exe;
        cat->driver.execute = cat_exe;
        echo->driver.execute = echo_exe;

        return PSH_OK;
    }
    return PSH_MOUNT_ERROR;
}
