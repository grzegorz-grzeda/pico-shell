#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "psh.h"
#include "utils/psh_cmd.h"
#include "ramdisc/psh_ramdisc.h"

int get_char(struct psh_file_t *file, char *buffer, int count)
{
    return read(0, buffer, count);
}

int put_char(struct psh_file_t *file, char *buffer, int count)
{
    return write(1, buffer, count);
}

int execute(struct psh_file_frame_t *f, int argc, char **argv)
{
    psh_print("In basic-command!\n");
}

#define MAX_NODES 100
#define BUF_SIZE 1000
#define PARAM_SIZE 10
#define MAX_RAMDISC_FILES 20
#define MAX_RAMDISC_FILE_SIZE 100
#define MAX_RAMDISC_DATA (MAX_RAMDISC_FILE_SIZE * MAX_RAMDISC_FILES)
psh_file nodes[MAX_NODES];
char inbuf[BUF_SIZE];
char *parambuf[PARAM_SIZE];
char ramdisc_data[MAX_RAMDISC_DATA];
psh_ramdisc ramdisc_buffer[MAX_RAMDISC_FILES];

psh_cli cli;

int main(void)
{
    cli.input.buffer = inbuf;
    cli.input.size = BUF_SIZE;
    cli.params.buffer = parambuf;
    cli.params.size = PARAM_SIZE;
    cli.delimiter = ' ';
    cli.new_line = '\n';
    cli.ps1 = "# ";

    psh_init_fs(nodes, MAX_NODES);
    psh_mount_std(get_char, put_char, put_char);
    psh_add_util_cmds();
    psh_init_ramdisc(ramdisc_data, MAX_RAMDISC_DATA, MAX_RAMDISC_FILES, ramdisc_buffer, MAX_RAMDISC_FILES);

    psh_file *cmd = psh_mount_file("basic-command", "Basic command to execute");
    if (cmd)
    {
        cmd->driver.execute = execute;
    }

    psh_attach_ramdisc_to_file(psh_mount_file("example-text-file", ""));
    psh_attach_ramdisc_to_file(psh_mount_file("text", ""));

    psh_init_cli(&cli);
    while (1)
    {
        psh_process();
    }
}
