#include "psh.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

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
    psh_print(proc->stdout, "In basic-command!\n");
}

int read_some_file(struct psh_node_t *file, char *buffer, int count)
{
    const char *text = "Some random text to display in PSH";
    int len = strlen(text);
    *buffer = text[file->_chars_read];
    return (len > file->_chars_read) ? 1 : 0;
}

#define BUF_SIZE 1000
#define PARAM_SIZE 10
char inbuf[BUF_SIZE];
char *parambuf[PARAM_SIZE];

psh_context cli;
psh_node root;
psh_file stdin_file;
psh_file stdout_file;
psh_node basic_command;
psh_node some_file;

int main(void)
{
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
}
