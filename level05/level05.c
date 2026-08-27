#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main(void)
{
    char buff[100];

    fgets(buff, 100, stdin);
    for (size_t i = 0; i < strlen(buff); i++)
        if (buff[i] >= 'A' && buff[i] <= 'Z')
            buff[i] = tolower(buff[i]);
    printf(buff);
    exit(0);
}
