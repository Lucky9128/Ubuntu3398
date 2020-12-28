#include<stdio.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include <errno.h>
#include<stdlib.h>
extern int errno ;
  
char dir[100];
char command[100]={'\0'};
int currpid,turn=1,shellId,InBack=1;
int ProInBack, ProInFront;
int ArrProInBack[1000][2];

void AttachSignal();
void RemoveBSignal();
char clr(char *c,int len)
{
	int i=0;
	for(int i=0;i<len;i++)
		*(c+i)='\0';
}
int parse(char *line,char **argv)
{
	int i=0;
	int j=0;
	while (*line != '\0') 
	{   
		if(*line=='\n')
		{	
			*line++='\0';
			return i;
		}
        while (*line == ' ' || *line == '\t')
		{
            *line++ = '\0';
			if(*line=='\n')
			{	
				*line++='\0';
				return i;
			}
		}
		*argv++= line;
		i++;
        while (*line != '\0' && *line != ' ' && *line != '\t' )
		{
            line++;
			if(*line=='\n')
			{	
				*line++='\0';
				return i;
			}
		}
		//printf(" arg[%i] : %s\n",i-1,*(argv-1));
    }
    *argv = '\0';  
	return i;
}
/*****************************function for background foreground*****************************************/
void print()
{
	for(int i=0;i<ProInBack;i++)
	{
		printf("%d %d %d\n",i,ArrProInBack[i][0],ArrProInBack[i][1]);
	}
}
void MoveProToback(int id)
{
	ArrProInBack[ProInBack][0]=id;
	ArrProInBack[ProInBack++][1]=0;
}

void SwitchPro(int id,int B2F)
{
	for(int i=0;i<ProInBack;i++)
	{
		if(ArrProInBack[i][0]==id)
		{	
			ArrProInBack[i][1]=B2F;
			kill(ArrProInBack[i][0]*(-1),18);
			break;
		}
	}
}

// void Reorder(int id)
// {
// 	int flag=-1;
// 	for(int i=0;i<ProInFront;i++)
// 	{
// 		if(ArrProInFront[i][0]==currpid)
// 		{
// 			flag=i;
// 		}
// 	}
// 	if(flag>=0)
// 	{
// 		ArrProInFront[flag]=ArrProInFront[ProInFront--];
// 	}
// }
/*****************************function for background foreground*****************************************/
int start(char **argv)
{
	fflush(stdin);
	clr(command,100);
	getcwd(dir,sizeof(dir));
	printf("%d@%s>",getpid(),dir);
	fgets(command,sizeof(command),stdin);
	//scanf("%s",command);
	// printf("\n");
	// printf("the command inputed: %sn\n",command);
	//clrargv(argv,sizeof(argv),sizeof(*argv));
	
	int count=parse(command,argv);
	//printf("count:%d \n ",count);
	int i=0;
	/*while(i<count)
	{
		printf("%d :",i);
		puts(argv[i]);
		i++;
	}*/
	if(strcmp(argv[0],"quit")==0||strcmp(argv[0],"Quit")==0||strcmp(argv[0],"Exit")==0||strcmp(argv[0],"EXIT")==0||strcmp(argv[0],"exit")==0||strcmp(argv[0],"QUIT")==0)
	{
		printf("exiting\n");
		return 1;
	}
	if(strcmp(argv[0],"jobs")==0)
	{
		print();
		return 2;
	}
	if(strcmp(argv[0],"fg")==0)
	{
		RemoveBSignal();
		int pid = ArrProInBack[0][0];
		ArrProInBack[0][0] = ArrProInBack[ProInBack-1][0];
		ArrProInBack[0][1] = ArrProInBack[ProInBack-1][1];
		ProInBack--;
		if(tcsetpgrp(STDIN_FILENO,pid)==0)
		{
			printf("Set grp\n");
		}
		else{
			printf("End grp\n");
		}
		printf("Terminal with %d\n",tcgetpgrp(STDIN_FILENO));
		waitpid(pid,NULL,WUNTRACED);

		AttachSignal();
		if(tcsetpgrp(STDIN_FILENO,shellId)==0)
		{
			printf("Set grp\n");
		}
		else{
			printf("End grp\n");
		}
		return 2;
	}
	else
		return 0;
}
void RemoveSignal()
{
	for(int i=1;i<=64;i++)
	{
		signal(i,SIG_DFL);
	}
}
int execute(char **argv)
{
	int ret;
	if((ret=fork())==0)
	{
		// RemoveSignal();
		//printf("child pid %d\n",getpid());

        setpgid(0,getpid());
		
		if(InBack)
			kill(getpid()*(-1),20);
		if(execvp(*argv, argv)<0)
		{
			//printf("command not found");
			int errnum = errno;
			fprintf(stderr, "Error %d : %s\n",errnum, strerror( errnum ));
			exit(0);
		}
		exit(0);
	}
	else
	{
		if(InBack)
		{
			MoveProToback(ret);
			sleep(1);
			waitpid(ret,NULL,WNOHANG);
			kill(ret*(-1),18);
			SwitchPro(ret,1);
			if(turn--<0)
			{
				InBack=0;
			}
		}
		else
		{
			currpid = ret;
			if(tcsetpgrp(STDIN_FILENO,currpid)==0)
			{
				printf("Set grp\n");
			}
			else{
				printf("End grp\n");
			}
			waitpid(ret,NULL,WUNTRACED);

			AttachSignal();
			if(tcsetpgrp(STDIN_FILENO,shellId)==0)
			{
				printf("Set grp\n");
			}
			else{
				printf("End grp\n");
			}
		}
		print();
		clr(command,100);
	}
}
void PassSignalToFg(int sigID)
{
	if(sigID==20)
	{
		if(currpid != shellId)
		{
			MoveProToback(currpid);
			kill(currpid*(-1),20);
		}
	}
	// if(sigID==2)
	// exit(0);
}
void AttachSignal()
{
	// for(int i=1;i<=64;i++)
	// {
		signal(SIGTTOU,SIG_IGN);
	// }
}

void RemoveBSignal()
{
	// for(int i=1;i<=64;i++)
	// {
		signal(SIGTTOU,SIG_DFL);
	// }
}

int main()
{
	shellId = getpid();
	currpid = shellId;
	int check=0;
	while(1)
	{
		char  *argv[100]={'\0'};
		check=start(argv);
		if(check==0)
		{
            execute(argv);
        }
		else if(check==2)
		{
            continue;
        }	
		else
			break;
	}
	exit(0);
}