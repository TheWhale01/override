#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>

int auth(char *buff, unsigned int nb)
{
    size_t sVar1;
    int iVar2;
    long lVar3;
    unsigned int u_local;

    sVar1 = strcspn(buff, "\n");
    buff[sVar1] = '\0';
    sVar1 = strnlen(buff, 32);
    if (sVar1 < 6 || ptrace(PTRACE_TRACEME) == -1)
        return 1;
    u_local = (buff[3] ^ 4919) + 6221293;
    for (int i = 0; i < sVar1; i++) {
        if (buff[i] < 32)
            return 1;
        u_local = u_local + (buff[i] ^ u_local) % 1337;
    }
    if (nb == u_local)
        return 0;
    return 1;
}

int main(void)
{ 
    int var1;
    int offset;
    char buff[32];
    int local_var;

    local_var = offset + 20;
    printf("-> Enter Login: ");
    fgets(buff, 32, stdin);
    printf("-> Enter Serial: ");
    unsigned int buff = scanf("%u", buff);
    int var1 = auth(buff, buff);
    if (var1 == 0) {
        puts("Authenticated!");
        system("/bin/sh");
    }
    if (local_var != offset + 20)
        __stack_chk_fail();
    return var1 != 0;
}
