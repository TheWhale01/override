#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>

int auth(char *buff, unsigned int serial)
{
    size_t len;
    unsigned int computed_serial;

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
    computed_serial = (buff[3] ^ 4919) + 6221293;
    for (int i = 0; i < len; i++) {
        if (buff[i] < 32)
            return 1;
        computed_serial += (buff[i] ^ computed_serial) % 1337;
    }
    if (serial == computed_serial)
        return 0;
    return 1;
}

int main(void)
{
    int computed_serial;
    char buff[32];
    int canary;
    unsigned int serial;

    puts("***********************************");
    puts("*\t\tlevel06\t\t  *");
    puts("***********************************");
    printf("-> Enter Login: ");
    fgets(buff, 32, stdin);
    puts("***********************************");
    puts("***** NEW ACCOUNT DETECTED ********");
    puts("***********************************");
    printf("-> Enter Serial: ");
    scanf("%u", &serial);
    computed_serial = auth(buff, serial);
    if (computed_serial == 0) {
        puts("Authenticated!");
        system("/bin/sh");
    }
    return computed_serial != 0;
}
