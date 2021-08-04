#ifndef PSH_H__
#define PSH_H__
/**
 * @addtogroup psh_group
 * @{
 */

typedef enum psh_node_type_t
{
    PSH_NODE_FILE = 0,
    PSH_NODE_DIR
} psh_node_type;

struct psh_node_t;
typedef int (*psh_file_io_cb)(struct psh_node_t *file, char *buffer, int count);
typedef int (*psh_file_exe_cb)(struct psh_node_t *file, int argc, char **argv);

struct psh_node_base_t;
typedef struct psh_node_base_t
{
    const char *name;
    psh_node_type _type;
    struct psh_node_base_t *_parent;
    struct psh_node_base_t *_next;
} psh_node_base;

typedef struct psh_file_node_t
{
    psh_node_base _base;
    const char *help;
    psh_file_io_cb read;
    psh_file_io_cb write;
    psh_file_exe_cb execute;
    struct psh_node_t *_file_descriptor_list_next;
    int _is_open;
} psh_file;

typedef struct psh_directory_node_t
{
    psh_node_base _base;
    psh_node_base *_children;
} psh_directory;

/**
 * Callback definition for pico-shell printing and reading function
 * @param pointer to character to be printed or read
 * @return 0 if operation failed
 * @return 1 if operation succeded
 */
typedef int (*psh_cli_io_cb)(char *);

typedef struct psh_cli_t
{
    struct
    {
        char *buffer;
        int size;
        psh_cli_io_cb io;
    } input;
    struct
    {
        char *buffer;
        int size;
        psh_cli_io_cb io;
    } output;
    psh_directory *root;
} psh_cli;

int psh_init(psh_cli *cli, psh_directory *root_node, psh_directory *bin_node);
int psh_mount_directory(psh_directory *parent, const char *name);
int psh_mound_file(psh_directory *parent, psh_file *file);

psh_file psh_open(const char *name);
int psh_write(psh_file fd, char *buffer, int count);
int psh_read(psh_file fd, char *buffer, int count);
int psh_close(psh_file fd);

int psh_process(psh_cli *cli);

/**
 * @}
 */
#endif