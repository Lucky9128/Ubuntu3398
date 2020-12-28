#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

void main()
{
    int p[2];

    pipe(p);

    if(fork()==0)
    {
        sleep(1);
        close(p[1]);
        dup2(p[0],0);
        
        char *const arg[] = {"t1",NULL};
        execvp("t1",arg);
    }
    close(p[0]);
    dup2(p[1],1);
    write(p[1],"hello Brother",14);
    sleep(1);
}