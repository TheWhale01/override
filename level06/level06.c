#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>

int auth(char *buff, unsigned int nb)
{
    size_t len;
    unsigned int auth_ret;

    buff[strcspn(buff, "\n")] = '\0';
    len = strnlen(buff, 32);
    if (len < 6)
        return 1;
    if (ptrace(PTRACE_TRACEME) == -1)
    {
        puts("\x1b[32m.---------------------------.");
        puts("\x1b[31m| !! TAMPERING DETECTED !!  |");
        puts("\x1b[32m\'---------------------------\'");
        return 1;
    }
    auth_ret = (buff[3] ^ 4919) + 6221293;
    for (int i = 0; i < len; i++) {
        if (buff[i] < 32)
            return 1;
        auth_ret += (buff[i] ^ auth_ret) % 1337;
    }
    if (nb == auth_ret)
        return 0;
    return 1;
}

int main(void)
{
    int auth_ret;
    // int *reg_gs;
    char buff[32];
    int canary;
    unsigned int nb;

    // canary = *(reg_gs + 20);
    puts("***********************************");
    puts("*\t\tlevel06\t\t  *");
    puts("***********************************");
    printf("-> Enter Login: ");
    fgets(buff, 32, stdin);
    puts("***********************************");
    puts("***** NEW ACCOUNT DETECTED ********");
    puts("***********************************");
    printf("-> Enter Serial: ");
    scanf("%u", &nb);
    auth_ret = auth(buff, nb);
    if (auth_ret == 0) {
        puts("Authenticated!");
        system("/bin/sh");
    }
    // if (canary != *(reg_gs + 20))
        // __stack_chk_fail();
        // return 1;
    return auth_ret != 0;
}
