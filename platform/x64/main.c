#include "psh.h"
#include <unistd.h>
#include <stdio.h>

int get_char(char *chr)
{
    return read(0, chr, 1);
}

int put_char(char *chr)
{
    return write(1, chr, 1);
}

int write_file(struct psh_node_t *file, char *buffer, int count)
{
    char buf[1000];
    snprintf(buf, count, "%s", buffer);
    printf("Writting to file %s: %d bytes: '%s'\n", file->name, count, buf);
}

#define BUF_SIZE 1000
char inbuf[BUF_SIZE];
char outbuf[BUF_SIZE];

psh_context cli;
psh_node root;
psh_node random_file;

int main(void)
{
    cli.input.buffer = inbuf;
    cli.input.size = BUF_SIZE;
    cli.input.io = get_char;
    cli.output.buffer = outbuf;
    cli.output.size = BUF_SIZE;
    cli.output.io = put_char;

    psh_init(&cli, &root);

    random_file.name = "/usr/random-file";
    random_file.help = "A random file";
    random_file.write = write_file;

    psh_mound_file(&root, &random_file);

    psh_file* f =psh_open(&cli, "/usr/random-file");
    if(f){
        psh_write(f, "Hello, World!", 13);
        psh_close(f);
    }
}
