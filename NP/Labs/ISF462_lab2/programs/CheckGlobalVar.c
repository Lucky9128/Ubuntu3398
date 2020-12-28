#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>

int glo = 15;

int main()
{
    int pi;
    int k =10;
    if((pi=vfork())<0)
        perror("Error");
    else if(pi==0)
        {
            glo++;
            k++;
            printf("Child : %d glo %d k\n",glo,k);
            exit(0);
        }
    else
    {
        // sleep(1);
       printf("Parent : %d glo %d k\n",glo,k);
    }
    return 0;
}