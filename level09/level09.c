#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void secret_backdoor(void)
{
    char buff[128];

    fgets(buff, 128, stdin);
    system(buff);
}

void set_msg(char *username)
{
    char buff[1024];

    memset(buff, 0, 128);
    puts(">: Msg @Unix-Dude");
    printf(">>: ");
    fgets(buff, 1024, stdin);
    strncpy(username, buff, (long)*(int*)(username + 180));
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
    printf(">: Welcome %s", (char *)offset + 140);
}

void handle_msg(void)
{
    char buff[140];
    long padding[5] = {0};
    int msg_len = 140;

    set_username((long)buff);
    set_msg(buff);
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
