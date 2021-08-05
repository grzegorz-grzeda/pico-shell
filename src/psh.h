#ifndef PSH_H__
#define PSH_H__
/**
 * @addtogroup psh_group
 * @{
 */

struct psh_node_t;

typedef struct psh_process_t{
    struct psh_node_t *file;
    struct psh_node_t *stdin;
    struct psh_node_t *stdout;
    struct psh_node_t *stderr;
}psh_process;

typedef int (*psh_file_opt_cb)(struct psh_node_t *file);
typedef int (*psh_file_io_cb)(struct psh_node_t *file, char *buffer, int count);
typedef int (*psh_file_exe_cb)(struct psh_process_t *process, int argc, char **argv);

typedef struct psh_node_t
{
    struct psh_node_t *_next;
    const char *name;
    const char *help;
    psh_file_opt_cb open;
    psh_file_opt_cb close;
    psh_file_io_cb read;
    psh_file_io_cb write;
    psh_file_exe_cb execute;
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
        int _cnt;
        const char* ps1;
        char delimiter;
        char new_line;
    } cli;
    psh_node *_root;
    struct psh_node_t *_stdin;
    struct psh_node_t *_stdout;
    struct psh_node_t *_stderr;
} psh_context;

int psh_init(psh_context *context, psh_node *root_node, psh_file *stdin, psh_file *stdout, psh_file *stderr);
int psh_mound_file(psh_context *context, psh_file *file);

psh_file *psh_open(psh_context *context, const char *name);
int psh_write(psh_file *fd, char *buffer, int count);
int psh_read(psh_file *fd, char *buffer, int count);
int psh_close(psh_file *fd);

int psh_print(psh_file *fd, const char *text);

int psh_execute(psh_context *context);

/**
 * @}
 */
#endif