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
    // signal(SIGUSR1,pri);
     FILE *fp;
        while(1)
        {
            fp = fopen("Backtrial2.txt","a");
            fputc('1',fp);
            fclose(fp);
            sleep(1);
        }

}