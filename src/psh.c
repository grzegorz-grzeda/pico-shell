#include "psh.h"
#include "utils/psh_utils.h"

int psh_init(psh_context *ctx, psh_node *root_node, psh_file *stdin, psh_file *stdout, psh_file *stderr)
{
    if (ctx != 0 && root_node != 0 && stdin != 0 && stdout != 0)
    {
        root_node->name = "/";
        root_node->help = "Root directory";
        root_node->read = 0;
        root_node->write = 0;
        root_node->execute = 0;
        root_node->_is_open = 0;
        root_node->_next = 0;
        ctx->_stdin = stdin;
        ctx->_stdin->_is_open = 1;
        ctx->_stdout = stdout;
        ctx->_stdout->_is_open = 1;
        ctx->_stderr = (stderr) ? stderr : stdout;
        ctx->_stderr->_is_open = 1;
        ctx->_root = root_node;
        ctx->cli._cnt = 0;
        psh_print(ctx->_stdout,ctx->cli.ps1);
    }
    else
    {
        return -1;
    }
}

int psh_mound_file(psh_context *context, psh_file *file)
{
    if (context && file)
    {
        psh_node *last_parent = context->_root;
        while (last_parent->_next)
        {
            last_parent = last_parent->_next;
        }
        last_parent->_next = file;
        file->_next = 0;
    }
    else
    {
        return -1;
    }
}

psh_file *psh_open(psh_context *context, const char *name)
{
    psh_node *candidate = context->_root;
    while (candidate)
    {
        if (are_strings_equal(candidate->name, name))
        {
            candidate->_is_open = 1;
            return candidate;
        }
        else
        {
            candidate = candidate->_next;
        }
    }
    return 0;
}

int psh_write(psh_file *fd, char *buffer, int count)
{
    if (fd->write != 0 && fd->_is_open)
    {
        return fd->write(fd, buffer, count);
    }
    else
    {
        return -1;
    }
}

int psh_read(psh_file *fd, char *buffer, int count)
{
    if (fd->read != 0 && fd->_is_open)
    {
        return fd->read(fd, buffer, count);
    }
    else
    {
        return -1;
    }
}

int psh_close(psh_file *fd)
{
    if (fd->_is_open)
    {
        fd->_is_open = 0;
        return 1;
    }
    else
    {
        return 0;
    }
}

int psh_print(psh_file *fd, const char *text)
{
    while (*text)
    {
        fd->write(fd, (char *)text++, 1);
    }
}

int psh_execute(psh_context *context)
{
    int ret_val = 0;
    char c;
    int result = context->_stdin->read(context->_root, &c, 1);

    if (result)
    {
        if (c == context->cli.new_line)
        {
            // parse & execute
            psh_print(context->_stdout, "\n");
            psh_print(context->_stdout, context->cli.ps1);
        }
        else
        {
            context->cli.buffer[context->cli._cnt++] = c;
        }
    }
    return ret_val;
}
