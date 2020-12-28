#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void getarg(char *arg)
{

}

void main()
{
    //Taking input
    char *cmd = (char *)malloc(sizeof(char)*200);
    fputs("Enter your command : > ",stdout);
    fgets(cmd,199,stdin);
    
    //filtering input
    char* loc =  strchr(cmd,'~');
    int len = (loc - cmd); 
    cmd[len]='\0';

    int child = fork();
    if(child==0)
    {
        exec(cmd,);
        exit(0);
    }


    // wait();
    // printf("I am done:\n");
}