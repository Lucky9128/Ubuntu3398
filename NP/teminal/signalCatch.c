#include<stdio.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include <errno.h>
#include<stdlib.h>
int cuur;
void sihg(int k)
{
    printf("signal recv %d by %d\n",k,getpid());
}
void main()
{
    for(int i=1;i<64;i++)
        signal(i,sihg);

    // if(fork()==0)
    // {
    //     setpgid(0,getpid());
    //     while(1);
    // }
    while(1);
    // wait(NULL);
}
  