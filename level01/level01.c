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
