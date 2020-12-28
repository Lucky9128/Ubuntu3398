#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
void main(int argc, char *argv[])
{
    int p1[2],p2[2];
    pipe(p1);
    pipe(p2);
    int pre = atoi(argv[1]);
    if(fork()==0)
    {
        close(p1[1]);
        dup2(p1[0],0);
        close(p2[0]);
        dup2(p2[1],1);
        dup2(p2[1],2);
        char ch[10]={0};
        char *const arg[] = {"2",pre,NULL};
        execvp("MQ",arg);
    }
    else if(fork()==0)
    {
        close(p1[0]);
        close(p2[0]);
        dup2(p2[1],1);
        dup2(p2[1],2);
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