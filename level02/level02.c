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
