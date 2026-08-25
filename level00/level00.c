#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SCANF_OP "%d"

bool main(void)
{
    int buff[4];

    puts("***********************************");
    puts("* \t     -Level00 -\t\t  *");
    puts("***********************************");
    printf("Password:");
    scanf(SCANF_OP, buff);
    if (buff[0] != 5276)
        puts("\nInvalid Password!");
    else
    {
        puts("\nAuthenticated!");
        system("/bin/sh");
    }
    return buff[0] != 5276;
}
