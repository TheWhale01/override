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
