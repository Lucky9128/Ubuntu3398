#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>

int main()
{
    if(fork())
    {
        printf("Helo\n");
        sleep(5);
        exit(0);
    }else{
        sleep(10);
    }
}