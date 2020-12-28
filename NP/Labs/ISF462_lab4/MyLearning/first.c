#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<signal.h>
void Hel()
{
    printf("Hello\n");
}
void main()
{
    signal(SIGUSR1,Hel);

    for(int i=0;i<5;i++)
    {
        if(fork()==0)
        {
            printf("Child No : %d \n",getpid());
            sleep(1);
            kill(getppid(),SIGUSR1);
            sleep(2);
        }
    }

}