#include "psh.h"
#include <unistd.h>

int get_char(char *chr)
{
    return read(0, chr, 1);
}

int put_char(char *chr)
{
    return write(1, chr, 1);
}

int main(void)
{
    char c;
    while (1)
    {
        if (get_char(&c))
            put_char(&c);
    }
}
