#include<stdio.h>
#include<sys/msg.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/signal.h>

int main()
{

    if(fork()==0)
    {
        close(0);
        close(1);
        close(2);
        setsid();
        if(fork()==0)
        {
            char *const arg[] = {"t6",NULL};
            execvp("t6",arg);
        }
        exit(0);
    }
    for(int i=0;i<64;i++)
        signal(i,SIG_IGN);
    exit(0);
}