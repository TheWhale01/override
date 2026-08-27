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
