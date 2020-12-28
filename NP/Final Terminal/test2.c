#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
void main()
{
    int p[2];
    pipe(p);
    if(fork()==0)
    {
        close(p[1]);
        dup2(p[0],0);
        char ch[10]={0};
        sprintf(ch,"%d",getppid());
        char *const arg[] = {"1",ch,NULL};
        execvp("MQ",arg);
    }
    close(p[0]);
    dup2(p[1],1);
    dup2(p[1],2);
    char *c = "Hello from ls\n";
    printf("%s",c);
    wait(NULL);
}