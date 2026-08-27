# Level 03

Voici le code source decompile:

```C
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
```
Ici notre mot de passe qui est un nombre est comme `hasher` a partir d'une string ``Q}|u\`sfg~sf{}|a3`` sur laquelle on applique un `XOR`. Le nombre passe a decrypt est forcement compris entre 1 et 21, il nous faut donc trouver lequel est celui qui va nous renvoyer la string `Congratulations`. Pour ce faire on prend le code binaire de `Q = 01010001` et celui de `C = 01000011` puis on resout l'equation `Q ^ ? = C` ce qui donne :  

Q = 01010001  
? = 00010010  
C = 01000011  

On a ? = 00010010 ce qui vaut 18 en base 10.
Il nous suffit de faire `1337d00d - 18 = 322424827`  
322424827 est donc notre mot de passe.

```bash
level03@OverRide:~$ ./level03
***********************************
*               level03         **
***********************************
Password:322424827
$ cat /home/users/level04/.pass
kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf
```
