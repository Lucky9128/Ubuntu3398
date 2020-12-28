#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>


int main()
{
    char *const arg[] = {"t2",NULL};
    execvp("t2",arg);
}