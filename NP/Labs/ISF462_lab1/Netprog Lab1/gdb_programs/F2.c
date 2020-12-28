#include<stdio.h>
#include<stdlib.h>

int run(int n)
{
    if(n<=1)
        return 1;
    return n*run(n-1); 
}
void main()
{
    int i=0,k=2,j=3;
    printf("%d\n",run(5));
}