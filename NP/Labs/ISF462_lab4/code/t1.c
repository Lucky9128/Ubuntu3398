#define _POSIX_SOURCE
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

main() {
    printf("Pid\tPPID\tPGID\tPSID\n");
    fork();
    printf("%d\t%d\t%d\t%d\n",getpid(),getppid(),getgid(),getsid(0));
}