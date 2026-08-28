#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>

void clear_stdin(void)
{
    int iVar1;

    do {
      iVar1 = getchar();
      if ((char)iVar1 == '\n') {
        return;
      }
    } while ((char)iVar1 != -1);
    return;
}

uint get_unum(void)
{
    uint nb;

    nb = 0;
    fflush(stdout);
    scanf("%u", &nb);
    clear_stdin();
    return nb;
}

bool read_number(uint *db)
{
    uint index;

    printf(" Index: ");
    index = get_unum();
    printf(" Number at data[%u] is %u\n", index, db[index]);
    return true;
}

bool store_number(uint *db)
{
    uint nb;
    uint index;

    printf(" Number: ");
    nb = get_unum();
    printf(" Index: ");
    index = get_unum();
    if ((index % 3) == 0 || (nb >> 24 == 183))
    {
        puts(" *** ERROR! ***");
        puts("   This index is reserved for wil!");
        puts(" *** ERROR! ***");
        return false;
    }
    db[index] = nb;
    return true;
}

int main(int ac, char **av, char **envp)
{
    uint db[100];
    char cmd[20];

    bzero(db, 100);
    for (size_t i = 0; av[i] != NULL; i++)
        bzero(av[i], strlen(av[i]));
    for (size_t i = 0; envp[i] != NULL; i++)
        bzero(envp[i], strlen(envp[i]));
    puts(
        "----------------------------------------------------\n  Welcome to wil\'s crappy number storage service!   \n----------------------------------------------------\n Commands:                                          \n    store - store a number into the data storage    \n    read  - read a number from the data storage     \n    quit  - exit the program                        \n----------------------------------------------------\n   wil has reserved some storage :>                 \n----------------------------------------------------\n"
    );
    while (true)
    {
        bool success_cmd = false;

        bzero(cmd, 20);
        printf("Input command: ");
        fgets(cmd, 20, stdin);
        cmd[strcspn(cmd, "\n")] = '\0';
        if (strncmp(cmd, "store", 5) == 0)
            success_cmd = store_number(db);
        else if (strncmp(cmd, "read", 4) == 0)
            success_cmd = read_number(db);
        else if (strncmp(cmd, "quit", 4) == 0)
            return 0;
        if (success_cmd)
            printf(" Completed %s command successfully\n", cmd);
        else
            printf(" Failed to do %s command\n", cmd);
    }
    return 0;
}
