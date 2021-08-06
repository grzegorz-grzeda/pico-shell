#include "fs/psh_fs.h"
#include "utils/psh_utils.h"

#define MIN_NODE_CNT 5

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
    files.frame.in = psh_mount_file(PSH_STD_IN_NAME, PSH_STD_IN_HELP);
    files.frame.out = psh_mount_file(PSH_STD_OUT_NAME, PSH_STD_OUT_HELP);
    files.frame.err = psh_mount_file(PSH_STD_ERR_NAME, PSH_STD_ERR_HELP);

    if (!files.frame.in || !files.frame.out || !files.frame.err)
    {
        return PSH_MOUNT_ERROR;
    }

    files.frame.in->driver.read = stdin_cb;
    files.frame.in->_file.is_special = 1;
    files.frame.out->driver.write = stdout_cb;
    files.frame.out->_file.is_special = 1;
    files.frame.err->driver.write = stderr_cb;
    files.frame.err->_file.is_special = 1;

    return PSH_OK;
}

psh_file *psh_mount_file(const char *name, const char *help)
{
    psh_file *f = psh_find_file(name);

    if (!f)
    {
        if (files.count < files.size)
        {
            f = files.buffer;
            while (f->_next)
            {
                f = f->_next;
            }
            f->_next = &files.buffer[files.count++];
            f = f->_next;
            f->name = name;
            f->help = help;
            f->_next = 0;
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

psh_file_frame *psh_get_file_frame(psh_file *file)
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
        if (fd->_file.is_special || fd->_file.is_open)
        {
            if (fd->driver.read)
            {
                int result = fd->driver.read(fd, buffer, count);
                if (result > 0)
                {
                    fd->_file.current_position += result;
                }
                return result;
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
        if (fd->_file.is_special || fd->_file.is_open)
        {
            if (fd->driver.write)
            {
                int result = fd->driver.write(fd, buffer, count);
                if (result > 0)
                {
                    fd->_file.current_position += result;
                }
                return result;
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
        if (fd->_file.is_special || fd->_file.is_open)
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
        if (fd->_file.is_special || fd->_file.is_open)
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