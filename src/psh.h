#ifndef PSH_H__
#define PSH_H__
/**
 * @addtogroup psh_group
 * @{
 */
struct psh_node_t;

typedef int (*psh_file_io_cb)(struct psh_node_t *file, char *buffer, int count);
typedef int (*psh_file_exe_cb)(struct psh_node_t *file, int argc, char **argv);

typedef struct psh_node_t
{
    const char *name;
    const char *help;
    psh_file_io_cb read;
    psh_file_io_cb write;
    psh_file_exe_cb execute;
    struct psh_node_t *_next;
    struct psh_node_t *_next_in_path;
    int _is_open;
} psh_node;

typedef psh_node psh_file;

/**
 * Callback definition for pico-shell printing and reading function
 * @param pointer to character to be printed or read
 * @return 0 if operation failed
 * @return 1 if operation succeded
 */
typedef int (*psh_cli_io_cb)(char *);

typedef struct psh_context_t
{
    struct
    {
        char *buffer;
        int size;
        psh_cli_io_cb io;
        int _cnt;
    } input;
    struct
    {
        char *buffer;
        int size;
        psh_cli_io_cb io;
        int _cnt;
    } output;
    psh_node *_path;
    psh_node *_root;
} psh_context;

int psh_init(psh_context *context, psh_node *root_node);
int psh_mound_file(psh_node *parent, psh_file *file);
int psh_add_to_path(psh_context *context, psh_node *path);

psh_file *psh_open(psh_context *context, const char *name);
int psh_write(psh_file *fd, char *buffer, int count);
int psh_read(psh_file *fd, char *buffer, int count);
int psh_close(psh_file *fd);

int psh_process(psh_context *cli);

/**
 * @}
 */
#endif