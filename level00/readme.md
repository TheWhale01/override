# Level 00

Voici le code source de ce niveau:

```C
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SCANF_OP "%d"

bool main(void)
{
    int buff[4];

    puts("***********************************");
    puts("* \t     -Level00 -\t\t  *");
    puts("***********************************");
    printf("Password:");
    scanf(SCANF_OP, buff);
    if (buff[0] != 5276)
        puts("\nInvalid Password!");
    else
    {
        puts("\nAuthenticated!");
        system("/bin/sh");
    }
    return buff[0] != 5276;
}
```

Ce programme va lire l'entree utilisateur la convertir en `int` et la stocker dans `buff[0]` grace a `scanf()`. Donc pour executer notre shell il nous suffit juste de rentrer `5276`:

```bash
level00@OverRide:~$ ./level00 
***********************************
* 	     -Level00 -		  *
***********************************
Password:5276

Authenticated!
$ cat /home/users/level01/.pass
uSq2ehEGT6c9S24zbshexZQBXUGrncxn5sD5QfGL
```
