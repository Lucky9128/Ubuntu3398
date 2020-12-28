#include<stdio.h>
#include<sys/wait.h>
#include<stdlib.h>

int main()
{
    FILE *fp = fopen("cp.c","r");
    char *buf = NULL;
    size_t linebuf =0;
    getline(&buf,&linebuf,fp);
    printf("%s %ld",buf,linebuf);
    getline(&buf,&linebuf,fp);
    printf("%s %ld",buf,linebuf);
    fclose(fp);
    return 0;
}