#include "psh.h"
#include "utils/psh_utils.h"

int psh_init(psh_context *cli, psh_node *root_node)
{
    if (cli != 0 && root_node != 0)
    {
        cli->input._cnt = 0;
        cli->output._cnt = 0;
        root_node->name = "/";
        root_node->help = "Root directory";
        root_node->read = 0;
        root_node->write = 0;
        root_node->execute = 0;
        root_node->_is_open = 0;
        root_node->_next = 0;
        root_node->_next_in_path = 0;
        cli->_root = root_node;
        cli->_path = root_node;
    }
    else
    {
        return -1;
    }
}

int psh_mound_file(psh_node *parent, psh_file *file)
{
    if (parent && file)
    {
        psh_node *last_parent = parent;
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
int psh_add_to_path(psh_context *context, psh_node *path)
{
    psh_node *last_in_path = context->_path;
    while (last_in_path->_next_in_path)
    {
        last_in_path = last_in_path->_next_in_path;
    }
    last_in_path->_next_in_path = path;
    path->_next_in_path = 0;
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

int psh_process(psh_context *cli)
{
}
