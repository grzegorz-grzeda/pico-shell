#include "psh.h"

int psh_init(psh_cli *cli, psh_directory *root_node, psh_directory *bin_node)
{
}

int psh_mount_directory(psh_directory *parent, const char *name)
{
}

int psh_mound_file(psh_directory *parent, psh_file *file)
{
}

psh_file psh_open(const char *name)
{
}

int psh_write(psh_file fd, char *buffer, int count)
{
}

int psh_read(psh_file fd, char *buffer, int count)
{
}

int psh_close(psh_file fd)
{
}

int psh_process(psh_cli *cli)
{
}
