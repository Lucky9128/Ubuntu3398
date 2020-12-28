#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main()
{
    if(fork()==0)
    {
        printf("Hello\n");
        execl("/home/lucky/Documents/Bits ME Sem1/NP/t1",(char *)NULL,(char *)NULL);        
        exit(0);
    }
    wait(NULL);
    return 0;
}