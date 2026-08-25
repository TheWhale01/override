#include <stdio.h>
#include <strings.h>

void verify_user_name()
{

}

int main(void)
{
    char buff[64];
    int nb;

    bzero(buff, 64);
    nb = 0;
    puts("********* ADMIN LOGIN PROMPT *********");
    printf("Enter Username: ");
    fgets(&a_user_name, 256, stdin);
}
