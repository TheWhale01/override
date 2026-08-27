#include <stdio.h>
#include <string.h>

int main(void)
{
    char buff[] = "Q}|u`sfg~sf{}|a3";

    for (size_t i = 0; i < 22; i++)
    {
        for (size_t j = 0; j < 16; j++)
            buff[j] = buff[j] ^ i;
        printf("i: %ld - strncmp: %d\n", i, strncmp(buff, "Congratulations!", 16));
        strcpy(buff, "Q}|u`sfg~sf{}|a3");
    }
}
