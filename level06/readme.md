# Level 06

Voici le code source de ce niveau:

```C
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
```

Grace a Ghidra nous pouvons egalement voir que ce code a ete compile avec `-fstack-protector`:

```C
int local_14
// ... 
local_14 = *(int *)(in_GS_OFFSET + 0x14);
// ...
if (local_14 != *(int *)(in_GS_OFFSET + 0x14)) {
  /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}
```

Le but ici est de rentrer une string en login, qui, une fois passee dans `auth()` nous donnera le nombre qu'on a passe en serial. Ici pas besoin de faire un calcul complexe, nous pouvons simplement ajouter un `printf()` a notre code source et le compiler pour voir la valeur de `computed_serial`:

```C
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>

int auth(char *buff, unsigned int serial)
{
    // ... Reste de la fonction
    for (int i = 0; i < len; i++) {
        if (buff[i] < 32)
            return 1;
        computed_serial += (buff[i] ^ computed_serial) % 1337;
    }
    printf("%d\n", computed_serial);
    if (serial == computed_serial)
        return 0;
    return 1;
}
```

Maintenant nous pouvons rentrer n'importe quelle valeur et regarder son resultat:

```bash
➜  level06 git:(main) gcc -fstack-protector ./level06.c && ./a.out
***********************************
*		level06		  *
***********************************
-> Enter Login: level06
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 123456
6233721

level06@OverRide:~$ ./level06 
***********************************
*		level06		  *
***********************************
-> Enter Login: level6
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: ^C
level06@OverRide:~$ ./level06 
***********************************
*		level06		  *
***********************************
-> Enter Login: level06
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 6233721
Authenticated!
$ cat /home/users/level07/.pass
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8
```
