#include<stdio.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include <errno.h>
#include<stdlib.h>
int cuur,end=1;

void sigh(int k)
{
    end=0;
    
}

void main()
{
    int ret1,ret2;
    // signal(SIGUSR1,sigh);
    if((cuur=fork())==0)
    {
        setpgid(0,getpid());
        kill(getpid(),20);
        FILE *fp;
        while(1)
        {
            fp = fopen("Backtrial.txt","a");
            fputc('1',fp);
            fclose(fp);
            sleep(1);
        }
        exit(0);
    }
    waitpid(cuur,NULL,WUNTRACED);
    kill(cuur*(-1),18);
    int i=100;
    while(i)
    {
        printf("3\n");
        sleep(1);
    }
}
  