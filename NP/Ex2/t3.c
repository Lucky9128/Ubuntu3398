#include<stdio.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include <errno.h>
#include<stdlib.h>


int getarg(char *fnames[],char* lct)
{
    int i=0;
    char *token; 
    while ((token = strtok_r(lct, " ", &lct))) 
    {
        fnames[i]=token;
        printf("T --> %s\n",fnames[i]);
        // fnames[i][strlen(fnames[i])-1] = '\0';
        printf("Filename --> %s\n",fnames[i]);
        i++;
    }

    return i;
}

void main()
{
    char cmm[] = "ls -l text.txt";
    char *cmd = cmm;
    char *fname[10];
    int k = getarg(fname,cmd);
    FILE *fp;
    while(k--)
    {
        fp= fopen(fname[k],"w");
        fclose(fp);
    }
}