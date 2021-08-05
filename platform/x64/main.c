#include "psh.h"
#include <unistd.h>
#include <stdio.h>

int get_char(struct psh_node_t *file, char *buffer, int count)
{
    return read(0, buffer, count);
}

int put_char(struct psh_node_t *file, char *buffer, int count)
{
    return write(1, buffer, count);
}

int execute(struct psh_process_t *proc, int argc, char **argv)
{
    char buf[1000];
}

#define BUF_SIZE 1000
char inbuf[BUF_SIZE];
char outbuf[BUF_SIZE];

psh_context cli;
psh_node root;
psh_file stdin_file;
psh_file stdout_file;
psh_node basic_command;

int main(void)
{
    stdin_file.read = get_char;
    stdout_file.write = put_char;
    cli.cli.buffer = inbuf;
    cli.cli.size = BUF_SIZE;
    cli.cli.ps1 = "#";
    cli.cli.delimiter = ' ';
    cli.cli.new_line = '\n';

    psh_init(&cli, &root, &stdin_file, &stdout_file, 0);

    basic_command.name = "basic-command";
    basic_command.help = "Basic command to execute";
    basic_command.execute = execute;

    psh_mound_file(&cli, &basic_command);

    psh_file *f = psh_open(&cli, "basic-command");
    if (f)
    {
        psh_write(f, "Hello, World!", 13);
        psh_close(f);
    }
    psh_execute(&cli);
}
