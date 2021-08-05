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
