#include<stdio.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include <errno.h>
#include<stdlib.h>
int cuur;
void main()
{
    if((cuur=fork())==0)
    {
        setpgid(0,getpid());
        while(1)
        {
            printf("2\n");
            sleep(1);
        }
    }
    sleep(2);
    waitpid(cuur,NULL,WUNTRACED);
    int i=100;
    while(i)
    {
        printf("3\n");
        sleep(1);
    }
}
  