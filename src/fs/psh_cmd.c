#include "fs/psh_cmd.h"
#include "utils/psh_utils.h"

static void print(psh_file *f, const char *text)
{
    psh_write(f, (char *)text, string_length(text));
}

int help_exe(struct psh_file_frame_t *frame, int argc, char **argv)
{
    print(frame->stdout, "Command list:\n");
    for (psh_file *f = frame->root; f != 0; f = f->_next)
    {
        if (!f->driver.execute)
        {
            continue;
        }
        print(frame->stdout, "  ");
        print(frame->stdout, f->name);
        print(frame->stdout, " - ");
        print(frame->stdout, f->help);
        print(frame->stdout, "\n");
    }
    return 0;
}

int ls_exe(struct psh_file_frame_t *frame, int argc, char **argv)
{
    print(frame->stdout, "Filesystem content:\n");
    for (psh_file *f = frame->root; f != 0; f = f->_next)
    {
        char mod[] = "---";
        mod[0] = f->driver.read ? 'r' : '-';
        mod[1] = f->driver.write ? 'w' : '-';
        mod[2] = f->driver.execute ? 'x' : '-';
        print(frame->stdout, "  ");
        print(frame->stdout, mod);
        print(frame->stdout, " ");
        print(frame->stdout, f->name);

        print(frame->stdout, "\n");
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
                psh_write(process->stdout, &c, 1);
            }
            psh_close(f);
        }
        else
        {
            print(process->stderr, "Can't find file '");
            print(process->stderr, argv[1]);
            print(process->stderr, "'");
        }
    }
    print(process->stderr, "\n");
    return 0;
}

int psh_add_util_cmds(void)
{
    psh_file *help = psh_mount_file("help", "Display this help");
    psh_file *ls = psh_mount_file("ls", "List pico-filesystem content");
    psh_file *cat = psh_mount_file("cat", "List content of a readable file");

    if (help && ls && cat)
    {
        help->driver.execute = help_exe;
        ls->driver.execute = ls_exe;
        cat->driver.execute = cat_exe;

        return PSH_OK;
    }
    return PSH_MOUNT_ERROR;
}
