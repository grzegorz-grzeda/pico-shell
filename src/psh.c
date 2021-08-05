#include "psh.h"
#include "cli/psh_cli.h"
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
        ctx->cli.input._cnt = 0;
        psh_cli_add_util_cmds(ctx);
        psh_print(ctx->_stdout, ctx->cli.ps1);
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

psh_file *psh_open(psh_node *root, const char *name)
{
    psh_node *candidate = root;
    while (candidate)
    {
        if (are_strings_equal(candidate->name, name))
        {
            candidate->_is_open = 1;
            candidate->_chars_read = 0;
            candidate->_chars_written = 0;
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
    int ret_val = -1;
    if (fd->write != 0 && fd->_is_open)
    {
        ret_val = fd->write(fd, buffer, count);
        if (ret_val > 0)
        {
            fd->_chars_written += ret_val;
        }
    }
    return ret_val;
}

int psh_read(psh_file *fd, char *buffer, int count)
{
    int ret_val = -1;
    if (fd->read != 0 && fd->_is_open)
    {
        ret_val = fd->read(fd, buffer, count);
        if (ret_val > 0)
        {
            fd->_chars_read += ret_val;
        }
    }
    return ret_val;
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
        if (context->cli.echo)
        {
            context->_stdout->write(context->_root, &c, 1);
        }
        if (c == context->cli.new_line)
        {
            context->cli.input.buffer[context->cli.input._cnt] = '\0';
            char *current_pos = context->cli.input.buffer;
            char *end_of_line = current_pos + context->cli.input._cnt;
            context->cli.cmd_params._cnt = 0;
            while ((context->cli.cmd_params._cnt < context->cli.cmd_params.size) && (current_pos < end_of_line))
            {
                while (*current_pos && (*current_pos == context->cli.delimiter))
                    current_pos++;

                if (current_pos > end_of_line)
                    break;

                context->cli.cmd_params.params[context->cli.cmd_params._cnt++] = current_pos;

                while (*current_pos && (*current_pos != context->cli.delimiter))
                    current_pos++;

                *(current_pos++) = '\0';
            }
            // search for execution
            psh_node *file = context->_root;
            while (file)
            {
                if (are_strings_equal(file->name, context->cli.cmd_params.params[0]))
                    break;
                else
                    file = file->_next;
            }
            // execute
            if (file && file->execute)
            {
                psh_process proc;
                proc.root = context->_root;
                proc.file = file;
                proc.stdin = context->_stdin;
                proc.stdout = context->_stdout;
                proc.stderr = context->_stderr;
                ret_val = file->execute(&proc, context->cli.cmd_params._cnt, context->cli.cmd_params.params);
            }
            else
            {
                ret_val = -1;
            }

            context->cli.input._cnt = 0;
            psh_print(context->_stdout, context->cli.ps1);
        }
        else
        {
            context->cli.input.buffer[context->cli.input._cnt++] = c;
        }
    }
    return ret_val;
}
