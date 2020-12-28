#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
void main(int arg,char *argv[])
{

    char ch[100];
    scanf("%s",ch);
    printf("MsgFromParent : %s\n",ch);
}