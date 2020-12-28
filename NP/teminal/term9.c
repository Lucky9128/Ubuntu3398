#include<stdio.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include <errno.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
extern int errno ;
int pip[1000][2];
char dir[100];
char command[10000]={'\0'};
char newcmd[10000]={'\0'};
char envi[10000]={'\0'};
char en[10000]={'\0'};



void removeSpaces(char *str) 
{ 
    int count = 0; 
	int i=0;
    while(str[i]!='\0') 
		i++;
    if(str[i-1]==' ')
		str[i-1] = '\0'; 
} 

int chkpipes(char *line)
{
		if(strchr(line,'|')!=NULL)
	{
		return 1;
	}
}

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
    }
    *argv = '\0';  
	return i;
}
int chkbg(char *line)
{
	char *sh=strchr(line,'&');
	if(sh!=NULL)
	{
		*sh='\0';
		if(*(sh-1)==' ')
			*(sh-1)='\0';
		return 1;
	}
}
int chkpip(char *line)
{
	char tes[10000];
	strcpy(tes,line);
	char *test[1000];
	int count=parse(tes,test);
	if(count<=0)
	{
		printf("error");
		return 4;
	}
	if(strcmp(test[0],"cd")==0)
	{
		char  *gdir;
		char  *dir;
		char  *to;
		char buf[100];
		if(test[1][0]=='/' || test[1][0]=='~')
		{
			printf("changing directory to%s \n",test[1]);
			if(chdir(test[1])!=0)
			{
				int errnum = errno;
				fprintf(stderr, "Error no %d : %s\n",errnum, strerror( errnum ));
			}
			return 4;
		}
		gdir = getcwd(buf, sizeof(buf));
		dir = strcat(gdir, "/");
		to = strcat(dir, test[1]);
		printf("changing directory to%s \n",to);
		if(chdir(to)!=0)
		{
			int errnum = errno;
			fprintf(stderr, "Error no %d : %s\n",errnum, strerror( errnum ));
		}
		return 4;
	}
	return 0;
}





int findcmd(char **cmd,char **ar) 
{
	char *dup = strdup(getenv("PATH"));
	char *s = dup;
	char *p = NULL;
	do {
		p = strchr(s, ':');
		if (p != NULL) {
			p[0] = 0;
		}
		char d[1000];
		// printf(" $PATH: %s\n", s);
		strcpy(d,s);
		strcat(d,"/");
		strcat(d,cmd[0]);
		// printf("chcecking in $PATH: %s\n", d);
		FILE *file;
		if( access( d, F_OK ) != -1 ) 
		{
			// printf("found :%s\n ",d);
			strcat(newcmd,d);
			int j=0;
			for(j=1;*(cmd+j);j++)
			{
				;
			}
			if(j>1)
				strcat(newcmd," ");
			int i=1;
			for(i=1;*(cmd+i);i++)
			{
				strcat(newcmd,*(cmd+i));
				if(i!=j-1)
					strcat(newcmd," ");
			}
			int count=parse(newcmd,ar);
			return 1;
			// file exists
		} else {
			// file doesn't exist
		}
		s = p + 1;
	} while (p != NULL);

	free(dup);
	return -1;
}


int getcmd()
{
	fflush(stdin);
	clr(command,10000);
	getcwd(dir,sizeof(dir));
	printf("%d@%s>",getpid(),dir);
	fgets(command,sizeof(command),stdin);
}


int chk(char *cmd,char **envp)
{
	char  *argv[10000]={'\0'};
	int count=parse(cmd,argv);
	int i=0;
	if(strcmp(argv[0],"quit")==0||strcmp(argv[0],"Quit")==0||strcmp(argv[0],"Exit")==0||strcmp(argv[0],"EXIT")==0||strcmp(argv[0],"exit")==0||strcmp(argv[0],"QUIT")==0)
	{
		printf("exiting\n");
		exit(0);
		return 1;
	}
}

int execfunc(char **argv,char **envp)
{
	if(execve(*argv, argv,envp)<0)
	{
		int errnum = errno;
		fprintf(stderr, "Error no %d : %s\n",errnum, strerror( errnum ));
		exit(0);
	}
}
int chld(char *cmd,char **envp)
{
	char  *oargv[10000]={'\0'};
	if(strchr(cmd,'.')!=NULL)
	{
		int count=parse(cmd,oargv);
		execfunc(oargv,envp);
	}
	else
	{
		int count=parse(cmd,oargv);
		char *argv[1000];
		int check=findcmd(oargv,argv);
		if(check==-1)
		{
			printf("command not found\n");
			exit(0);
		}
		else
		{

			execfunc(argv,envp);
		}
	}
	
}


int splitoredirect(char *command,char **args,char  *x)
{
	char cmd[10000];
	strcpy(cmd,command);
	char *ptr = strtok(cmd, x);
	int i=0;
	while(ptr != NULL)
	{
		*(args+i)=(char * ) malloc(1000*sizeof(char));
		strcpy(args[i],ptr);
		ptr = strtok(NULL, x);
		i++;
	}
	return i;
}
int redirecti(char *cmd,char **envp)
{
	char *args[1000];
	int count=splitoredirect(cmd,args,"<");
	// printf("count%d \n",count);
	// for(int i=0;i<count;i++)
		// puts(args[i]);
	if(count!=2&&count!=1)
		printf("error in syntax : %s \n ",cmd);
	else
	{
		if(count==2)
		{
			close(0);
			char filename[1000];
			for(int i=0;args[1][i];i++)
			{
				filename[i]=args[1][i];
			}
			removeSpaces(filename);
			int fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
			dup2(fd, 0);
			// close(fd);    
			char cm[10000];
			strcpy(cm,args[0]);
			removeSpaces(cm);
			// printf("filename : %s \n ",filename);
			// printf("comamnd %s \n",cm);
			chld(cm,envp);
		}
		else
		{
			chld(args[0],envp);
		}
	}

	// printf("count %d \n ",count);
	//splitoexecute(args,count,env);
}


int redirecto(char *cmd,char **envp)
{
	char *args[1000];
	int count=splitoredirect(cmd,args,">");
	// printf("count%d \n",count);
	// for(int i=0;i<count;i++)
		// puts(args[i]);
	if(count!=2&&count!=1)
		printf("error in syntax : %s \n ",cmd);
	else
	{
		if(count==2)
		{
			close(1);
			char filename[1000];
			strcpy(filename,args[1]);
			removeSpaces(filename);
			int fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
			if(!fd)
				printf("error openning file");
			dup2(fd, 1);
			// close(fd);    
			char cm[10000];
			strcpy(cm,args[0]);
			removeSpaces(cm);
			redirecti(cm,envp);
		}
		else
		{
			redirecti(args[0],envp);
		}
	}

	// printf("count %d \n ",count);
	//splitoexecute(args,count,env);
}
int exe(char *cmd,int x,int y,char **envp)
{
	char arg[1000];
	strcpy(arg,cmd);
	if(chk(cmd,envp)==1)
		return 0;
	int ret;
	if((ret=fork())==0)
	{
		if(x!=-1)
		{
			close(pip[x][1]);
			close(0);
			dup2(pip[x][0],0);
		}
		if(y!=-1)
		{
			close(pip[y][0]);
			close(1);
			dup2(pip[y][1],1);
		}
		redirecto(arg,envp);
		// fgbg(arg,envp);
		exit(0);
	}

	return ret;
}


int splitpipe(char *cm,char **args)
{
	char cmd[10000];
	strcpy(cmd,cm);
	char *ptr = strtok(cmd, "|");
	int i=0;
	while(ptr != NULL)
	{
		*(args+i)=(char * ) malloc(1000*sizeof(char));
		strcpy(args[i],ptr);
		ptr = strtok(NULL, "|");
		i++;
	}
	return i;
}

int pipeexecute(char **args,int count,char **envp,int bg)
{
	int i=0;
	pid_t ret[1000];
	for(i=0;i<count;i++)
	{
		printf("%s\n",args[i]);
		if(i>1)
		{
			close(pip[i-2][0]);
			close(pip[i-2][1]);
		}	
		pipe(pip[i]); 
		removeSpaces(args[i]);
		int l=0;
		int x=-1;
		int y=-1;
		if(i!=0)
			x=i-1;
		if(i!=count-1)
			y=i;
		ret[i]=exe(args[i],x,y,envp);
	}
	if(count>1)
	{
		close(pip[i-2][0]);
		close(pip[i-1][0]);
		close(pip[i-2][1]);
		close(pip[i-1][1]);
	}
	i=0;
	for(i=0;i<count;i++)
	{
		int status;
		if(bg==1)
			waitpid(ret[i],&status,WNOHANG);
		else
			waitpid(ret[i],&status,0);
	}
		 //parent code
	 //and here
	 //lucky
	 //put here only
	 //above wait pid are written
	clr(command,10000);
}


int pipexe(char *cmd,char **env,int bg)
{
	char *args[1000];
	int count=splitpipe(cmd,args);
	// printf("count %d \n ",count);
	pipeexecute(args,count,env,bg);
}
int fgbg(char *cmd,char **envp)
{
	int bg=0;
	printf("%s\n",cmd);
	if(chkbg(cmd)==1)
	{
		printf("background\n");
		//put your child code here
		//lucky
		bg=1;
	}
	pipexe(cmd,envp,bg);
}
int main(int argc,char *arg[],char *envp[])
{
	// int i;
    // for (i = 0; envp[i] != NULL; i++) 
        // printf("\n%s", envp[i]); 
	while(1)
	{
		getcmd();
		int set=chkpip(command);
		if(set==0)
		{
			fgbg(command,envp);
			// execute(envp);
		}
	}
	exit(0);
}