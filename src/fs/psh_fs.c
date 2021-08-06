#include "fs/psh_fs.h"
#include "utils/psh_utils.h"

#define MIN_NODE_CNT 5
#define STD_IN_HELP "pSH standard input"
#define STD_OUT_HELP "pSH standard output"
#define STD_ERR_HELP "pSH standard error output"

static struct
{
    psh_file *buffer;
    int count;
    int size;
    psh_file_frame frame;
} files;

int psh_init_fs(psh_file *buffer, int size)
{
    files.buffer = buffer;
    files.count = 0;
    files.size = size;
    files.frame.root = buffer;

    if (!buffer || (size < MIN_NODE_CNT))
    {
        return PSH_ERROR_MEM;
    }
    else
    {
        return PSH_OK;
    }
}

int psh_mount_std(psh_file_io_cb stdin_cb, psh_file_io_cb stdout_cb, psh_file_io_cb stderr_cb)
{
    files.frame.stdin = psh_mount_file("stdin", STD_IN_HELP);
    files.frame.stdout = psh_mount_file("stdout", STD_OUT_HELP);
    files.frame.stderr = psh_mount_file("stderr", STD_ERR_HELP);

    if (!files.frame.stdin || !files.frame.stdout || !files.frame.stderr)
    {
        return PSH_MOUNT_ERROR;
    }

    files.frame.stdin->driver.read = stdin_cb;
    files.frame.stdout->driver.write = stdout_cb;
    files.frame.stderr->driver.write = stderr_cb;

    return PSH_OK;
}

psh_file *psh_mount_file(const char *name, const char *help)
{
    psh_file *f = psh_find_file(name);

    if (!f)
    {
        if (files.count < files.size)
        {
            psh_file *f = files.buffer;
            while (f->_next)
            {
                f = f->_next;
            }
            f->_next = &files.buffer[files.count++];
            f->_next->name = name;
            f->_next->help = help;
            f->_next->_next = 0;
        }
    }

    return f;
}

psh_file *psh_find_file(const char *name)
{
    for (int i = 0; i < files.count; i++)
    {
        if (are_strings_equal(name, files.buffer[i].name))
        {
            return &files.buffer[i];
        }
    }

    return 0;
}

psh_file_frame *psh_get_node_frame(psh_file *file)
{
    files.frame.file = file;
    return &files.frame;
}

psh_file *psh_open(const char *name)
{
    psh_file *file = psh_find_file(name);

    if (file)
    {
        file->_file.current_position = 0;
        file->_file.is_open = 1;
        if (file->driver.open)
        {
            file->driver.open(file);
        }
    }

    return file;
}

int psh_read(psh_file *fd, char *buffer, int count)
{
    if (fd)
    {
        if (fd->_file.is_open)
        {
            if (fd->driver.read)
            {
                return fd->driver.read(fd, buffer, count);
            }
            return PSH_OK;
        }
        return PSH_FILE_NOT_OPEN;
    }
    return PSH_ERROR_MEM;
}

int psh_write(psh_file *fd, char *buffer, int count)
{
    if (fd)
    {
        if (fd->_file.is_open)
        {
            if (fd->driver.write)
            {
                return fd->driver.write(fd, buffer, count);
            }
            return PSH_OK;
        }
        return PSH_FILE_NOT_OPEN;
    }
    return PSH_ERROR_MEM;
}

int psh_seek(psh_file *fd, int count, int whence)
{
    if (fd)
    {
        if (fd->_file.is_open)
        {
            if (fd->driver.seek)
            {
                return fd->driver.seek(fd, count, whence);
            }
            return PSH_OK;
        }
        return PSH_FILE_NOT_OPEN;
    }
    return PSH_ERROR_MEM;
}

int psh_close(psh_file *fd)
{
    if (fd)
    {
        if (fd->_file.is_open)
        {
            int result = PSH_OK;
            if (fd->driver.close)
            {
                result = fd->driver.close(fd);
            }
            fd->_file.is_open = 0;
            return result;
        }
        return PSH_FILE_NOT_OPEN;
    }
    return PSH_ERROR_MEM;
}