#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
int val = 0;
FILE *fp;
void sighn2(int a)
{
    fp = fopen("log.txt","a");
    fprintf(fp,"%d\t%d\t%d\t%d\n",getpid(),getppid(),getpgrp(),getsid(0));
    fclose(fp);
    exit(0);
}
void pri(int a)
{
    printf("Signal is passed %d to %d\n",a,getpid());
}
void main()
{
    signal(SIGUSR1,pri);
    for(int i =0;i<=4;i++)
    {
        if(fork()==0)
        {
            sleep(50);
        }
    }
    sleep(50);
}