#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
void main()
{
    // signal(SIGUSR1,pri);
     FILE *fp;
        while(1)
        {
            fp = fopen("Backtrial.txt","a");
            fputc('1',fp);
            fclose(fp);
            sleep(1);
        }

}