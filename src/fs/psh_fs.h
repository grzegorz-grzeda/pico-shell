#ifndef PSH_FS_H__
#define PSH_FS_H__

#include "utils/psh_errors.h"

#define PSH_STD_IN_NAME "in"
#define PSH_STD_IN_HELP "pSH standard input"
#define PSH_STD_OUT_NAME "out"
#define PSH_STD_OUT_HELP "pSH standard output"
#define PSH_STD_ERR_NAME "err"
#define PSH_STD_ERR_HELP "pSH standard error output"

struct psh_file_t;

typedef struct psh_file_frame_t
{
    struct psh_file_t *root;
    struct psh_file_t *file;
    struct psh_file_t *in;
    struct psh_file_t *out;
    struct psh_file_t *err;
} psh_file_frame;

typedef enum psh_lsk_whence_t
{
    PSH_SEEK_SET = 0,
    PSH_SEEK_CURRENT,
    PSH_SEEK_END
} psh_lsk_whence;

typedef int (*psh_file_opt_cb)(struct psh_file_t *file);
typedef int (*psh_file_lsk_cb)(struct psh_file_t *file, int count, int whence);
typedef int (*psh_file_io_cb)(struct psh_file_t *file, char *buffer, int count);
typedef int (*psh_file_exe_cb)(struct psh_file_frame_t *process, int argc, char **argv);

typedef struct psh_file_t
{
    const char *name;
    const char *help;
    struct
    {
        psh_file_opt_cb open;
        psh_file_opt_cb close;
        psh_file_lsk_cb seek;
        psh_file_io_cb read;
        psh_file_io_cb write;
        psh_file_exe_cb execute;
    } driver;
    struct
    {
        int is_special;
        int is_open;
        int current_position;
        int size;
    } _file;
    struct psh_file_t *_next;
} psh_file;

int psh_init_fs(psh_file *buffer, int size);

int psh_mount_std(psh_file_io_cb stdin_cb, psh_file_io_cb stdout_cb, psh_file_io_cb stderr_cb);

psh_file *psh_mount_file(const char *name, const char *help);
psh_file *psh_find_file(const char *name);
psh_file_frame *psh_get_file_frame(psh_file *file);

psh_file *psh_open(const char *name);
int psh_read(psh_file *fd, char *buffer, int count);
int psh_write(psh_file *fd, char *buffer, int count);
int psh_seek(psh_file *fd, int count, int whence);
int psh_close(psh_file *fd);

#endif
