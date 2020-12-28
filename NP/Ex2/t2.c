
#include<stdio.h>
#include<stdlib.h>
#include<error.h>
#include<string.h>
#include<unistd.h>

extern int errno;
extern char **environ;

void main()
{
    char *const arg[] = {"ls",NULL};//NULL is necessary
    char *const env[] = {"./1","Lucky","hello",NULL};//NULL is necessary
//    execl("/home/lucky/Documents/Bits ME Sem1/NP/Practice/1","./1","Lucky","Hello",(char *)NULL);
//   execlp("/home/lucky/Documents/Bits ME Sem1/NP/Practice/1",".1/","Lucky","Hello",(char *)NULL);
//   execle("/home/lucky/Documents/Bits ME Sem1/NP/Practice/1",".1/","Lucky","Hello",(char *)NULL,arg);
//    execv("/home/lucky/Documents/Bits ME Sem1/NP/Practice/1",arg);
//   execvp("/home/lucky/Documents/Bits ME Sem1/NP/Practice/1",arg);
        if(execvp("ls", arg)<0)
		{
			//printf("command not found");
			// int errnum = errno;
			fprintf(stderr, "Error \n");
			exit(0);
		}
        //    errExit("excev");
}