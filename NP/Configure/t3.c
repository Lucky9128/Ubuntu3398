#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

void main()
{
    int p1[2],p2[2];

    pipe(p1);
    pipe(p2);
    //p1->filedes
    //p2->filed
    if(fork()==0)
    {
        close(p1[0]);
        close(p2[1]);
        dup2(p1[1],1);
        dup2(p2[0],0);
        char *const arg[] = {"t1",NULL};
        execvp("t1",arg);
    }
    close(p2[0]);
    close(p1[1]);
    write(p2[1],"HelloFromParent\n",50);
    char ch[100]={0};
    read(p1[0],ch,100);
    printf("Value by child : %s\n",ch);
}