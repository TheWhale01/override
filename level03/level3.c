#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

void decrypt(int key)
{
    char buff[] = "Q}|u`sfg~sf{}|a3";
    int len = strlen(buff);

    for (int i = 0; i < len; i++)
    {
        buff[i] = buff[i] ^ key;
    }
    if (strncmp(buff, "Congratulations!", 16) == 0)
    {
        system("/bin/sh");
        return;
    }
    puts("\nInvalid Password");
}

void test(int user_pass, int pass)
{
    int diff = pass - user_pass;

    if (diff >= 1 && diff <= 21)
        decrypt(diff);
    else
        decrypt(rand());
}

int main(void)
{
    uint seed;
    int user_pass;

    seed = time((time_t *)NULL);
    srand(seed);
    srand(seed);
    puts("***********************************");
    puts("*\t\tlevel03\t\t**");
    puts("***********************************");
    printf("Password:");
    scanf("%d", &user_pass);
    test(user_pass, 0x1337d00d);
    return 0;
}
