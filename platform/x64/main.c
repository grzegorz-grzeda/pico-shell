#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "psh.h"
#include "utils/psh_cmd.h"

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

int read_some_file(struct psh_file_t *file, char *buffer, int count)
{
    const char *text = "Some random text to display in PSH";
    int len = strlen(text);
    *buffer = text[file->_file.current_position];
    return (len > file->_file.current_position) ? 1 : 0;
}

#define MAX_NODES 100
#define BUF_SIZE 1000
#define PARAM_SIZE 10
psh_file nodes[MAX_NODES];
char inbuf[BUF_SIZE];
char *parambuf[PARAM_SIZE];

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
    psh_init_cli(&cli);
    while (1)
    {
        psh_process();
    }

    /*
    stdin_file.read = get_char;
    stdout_file.write = put_char;
    cli.cli.input.buffer = inbuf;
    cli.cli.input.size = BUF_SIZE;
    cli.cli.cmd_params.params = parambuf;
    cli.cli.cmd_params.size = PARAM_SIZE;
    cli.cli.ps1 = "# ";
    cli.cli.delimiter = ' ';
    cli.cli.new_line = '\n';
    cli.cli.echo = 0;
    psh_init(&cli, &root, &stdin_file, &stdout_file, 0);

    basic_command.name = "basic-command";
    basic_command.help = "Basic command to execute";
    basic_command.execute = execute;
    psh_mound_file(&cli, &basic_command);

    some_file.name = "some-file";
    some_file.help = "Some random file to read";
    some_file.read = read_some_file;
    psh_mound_file(&cli, &some_file);

    while (1)
        psh_execute(&cli);
        */
}
