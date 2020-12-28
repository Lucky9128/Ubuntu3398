#include<stdio.h>

void main()
{
    char ch[100]={0};
    FILE *fp = fopen("/proc/13419/cmdline","r");
    fscanf(fp,"%s",ch);
    printf("%s\n",ch);
}