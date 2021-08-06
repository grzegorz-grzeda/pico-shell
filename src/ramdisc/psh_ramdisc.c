#include "ramdisc/psh_ramdisc.h"
#include "utils/psh_utils.h"

static struct
{
    char *ptr;
    int size;
    int max_file_size;
} data;

static struct
{
    psh_ramdisc *ptr;
    int size;
    int cnt;
} ramdisc;

static psh_ramdisc *get_entry(psh_file *f)
{
    for (int i = 0; i < ramdisc.cnt; i++)
    {
        if (f == ramdisc.ptr[i].file)
        {
            return &ramdisc.ptr[i];
        }
    }
    return 0;
}

static int ramdisc_open_cb(struct psh_file_t *file)
{
    return 0;
}
static int ramdisc_close_cb(struct psh_file_t *file) { return 0; }
static int ramdisc_seek_cb(struct psh_file_t *file, int count, int whence)
{
    psh_ramdisc *rd = get_entry(file);

    if (rd)
    {
        switch (whence)
        {
        case PSH_SEEK_CURRENT:
            rd->buffer._written_cnt += count;
            rd->buffer._read_cnt += count;
            rd->file->_file.current_position += count;
            break;
        case PSH_SEEK_SET:
            rd->buffer._written_cnt = count;
            rd->buffer._read_cnt = count;
            rd->file->_file.current_position = count;
            break;
        case PSH_SEEK_END:
            rd->buffer._written_cnt = rd->buffer.size += count;
            rd->buffer._read_cnt = rd->buffer.size += count;
            rd->file->_file.current_position = rd->buffer.size + count;
            break;
        }
    }

    return 0;
}
static int ramdisc_read_cb(struct psh_file_t *file, char *buffer, int count)
{
    int cnt = 0;
    psh_ramdisc *rd = get_entry(file);

    if (rd)
    {
        if (rd->buffer._read_cnt >= rd->buffer._written_cnt)
        {
            return 0;
        }

        while ((cnt < count) && ((rd->file->_file.current_position + cnt) < rd->buffer._written_cnt))
        {
            buffer[cnt] = rd->buffer.ptr[rd->file->_file.current_position + cnt];
            cnt++;
            rd->buffer._read_cnt++;
        }
    }

    return cnt;
}
static int ramdisc_write_cb(struct psh_file_t *file, char *buffer, int count)
{
    int cnt = 0;
    psh_ramdisc *rd = get_entry(file);

    if (rd)
    {
        while ((cnt < count) && ((rd->file->_file.current_position + cnt) < rd->buffer.size))
        {
            rd->buffer.ptr[rd->file->_file.current_position + cnt] = buffer[cnt];
            cnt++;
            rd->buffer._written_cnt++;
        }
    }

    return cnt;
}

int psh_init_ramdisc(char *data_buffer, int data_size, int max_file_size, psh_ramdisc *ramdisc_buffer, int ramdisc_size)
{
    if (!data_buffer || !ramdisc_buffer)
    {
        return PSH_ERROR_MEM;
    }

    data.ptr = data_buffer;
    data.size = data_size;
    data.max_file_size = max_file_size;
    ramdisc.ptr = ramdisc_buffer;
    ramdisc.size = ramdisc_size;
    ramdisc.cnt = 0;

    return PSH_OK;
}

int psh_attach_ramdisc_to_file(psh_file *f)
{
    if (!f || !ramdisc.ptr || ramdisc.cnt >= ramdisc.size)
    {
        return PSH_ERROR_MEM;
    }

    if (data.size < data.max_file_size * (ramdisc.cnt + 1))
    {
        return PSH_ERROR_MEM;
    }

    ramdisc.ptr[ramdisc.cnt].file = f;
    ramdisc.ptr[ramdisc.cnt].buffer.ptr = data.ptr + (data.max_file_size * (ramdisc.cnt + 1));
    ramdisc.ptr[ramdisc.cnt].buffer.size = data.max_file_size;
    ramdisc.ptr[ramdisc.cnt].buffer._written_cnt = 0;
    ramdisc.ptr[ramdisc.cnt].buffer._read_cnt = 0;

    f->driver.open = ramdisc_open_cb;
    f->driver.close = ramdisc_close_cb;
    f->driver.read = ramdisc_read_cb;
    f->driver.write = ramdisc_write_cb;
    f->driver.seek = ramdisc_seek_cb;

    ramdisc.cnt++;

    return PSH_OK;
}