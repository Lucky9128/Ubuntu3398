#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include<sys/signal.h>
int end=49;
void sigh(int k)
{
    end++;
}
int main(int args,char **argv) {
   int i=5;
   while(i--)
   {
        printf("%d\n",i);
        sleep(1);
   } 
}