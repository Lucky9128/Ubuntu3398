#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>

void forkme(int n)
{

    printf("%d Hello\n",n);
    

}

int main(int argc, char *argv[])
{ 
    forkme(atoi(argv[1]));
    // int pid1,pid2;
    // printf("1\n");
    // pid1=fork();
    // if(pid1==0) 
    // {
    //     printf("2\n");
    //     pid2=fork();
    //     printf("2\t2\n");
    // } 
    // else
    // {
    //     printf("3\n");
    // }
}