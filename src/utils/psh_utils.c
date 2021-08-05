#include "utils/psh_utils.h"

int are_strings_equal(const char *s1, const char *s2)
{
    while (*s1 == *s2)
    {
        if (!*s1)
            return 1;
        else
        {
            s1++;
            s2++;
        }
    }
    return 0;
}

int string_length(const char *s0)
{
    int len = 0;
    while (s0[len++])
        ;
    return len;
}
