#include<stdio.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include <errno.h>
#include<stdlib.h>


void main()
{
    int p[2];
    pipe(p);
    if(fork()==0)
    {
        close(p[0]);
        dup2(p[1],1);
        char *ac[] = {"ls",NULL};
        execvp("ls",ac);
        exit(0);
    }
    close(p[1]);
    char *ch = (char *)malloc(sizeof(char)*1000);
    read(p[0],ch,1000);
    printf("%s",ch);
}