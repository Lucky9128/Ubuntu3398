#include<stdio.h>
#include<signal.h>
#include<string.h>
long n;
void sigalrm(int signo)
{
    alarm(1);
    n=n+1;
    printf("%ld seconds elapsed\n",n);
}
void main(int argv,char *argc[])
{   
    signal(SIGALRM,sigalrm);
    alarm(atoi(argc[1])); //It holds for 10 seconds and then send a signal alarm to process
    pause();
    printf("Signal Arrived\n");
}