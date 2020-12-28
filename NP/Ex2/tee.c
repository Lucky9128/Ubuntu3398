#include<stdio.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include <errno.h>
#include<stdlib.h>
extern int errno ;

int getarg(char *fnames[],char* lct)
{
    int i=0;
    char *token; 
    while ((token = strtok_r(lct, " ", &lct))) 
    {
        fnames[i]=token;
        // printf("T --> %s\n",fnames[i]);
        // fnames[i][strlen(fnames[i])-1] = '\0';
        // printf("Filename --> %s\n",fnames[i]);
        i++;
    }
    
    if(fnames[i-1][strlen(fnames[i-1])-1]=='\n')
    {
        fnames[i-1][strlen(fnames[i-1])-1]='\0';
    }
    // printf("%d\n",i);
    return i;
}

void main()
{
    //Taking input
    char *cmd = (char *)malloc(sizeof(char)*200);
    fputs("Enter your command : > ",stdout);
    fgets(cmd,199,stdin);
    
    //filtering input
    char* loc =  strchr(cmd,'~');
    int len = (loc - cmd); 
    while(cmd[--len]==32);
    cmd[len+1]='\0';
    char *args = loc+1;
    while(*args==' '||*args=='\t')
        args++;
    char *filename[10];
    //Making Pipe 
    int p[2];
    pipe(p);
    
    int child = fork();
    if(child==0)
    {
        close(p[0]);
        // printf("Commd : %s\n",cmd);
        int args = getarg(filename,cmd);
        filename[args] = (char *)NULL;
        dup2(p[1],1);
       if(execvp(filename[0], filename)<0)
	{
	//printf("command not found");
	int errnum = errno;
	fprintf(stderr,"Error %d : %s\n",errnum, strerror( errnum ));
	exit(0);
	}
    }
    close(p[1]);
    int *stt;
    wait(stt);
    int numargs = getarg(filename,args);
    FILE *fp;
    // printf("%d\n",numargs);
    char *data = (char *)malloc(sizeof(char)*4000);
    read(p[0],data,4000);    
    for(int i=0;i<numargs;i++)
    {
        // printf("Files -> %s\n",filename[i]);
        fp = fopen(filename[i],"w");
        if(fp==NULL)
        {
            perror("Error in file creation\n");
        }
        fputs(data,fp);
        fclose(fp);
    }
    printf("%s\n",data);
}
