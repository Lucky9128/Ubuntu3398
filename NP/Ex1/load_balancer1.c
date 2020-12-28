#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <string.h>
#include <ctype.h>
#include <errno.h> 
#include <sys/wait.h> 
int fl = 0;
int end = 0,fend=0;
int *cid,*stt;
int NumProcess;

FILE *fp;


void display()
{
    printf("Current Status : [");
    for(int i=0;i<NumProcess;i++)
    {
        printf("%d ",stt[i]);
    }
    printf("]\n");
    
}

//Signal Handler from parent to child
void sig(int k)
{
    fl=1;    
}

//Signal Handler from child to itself to go in waiting mode
void sig2(int k)
{
    end=0;    
    fl=0;
}


//Signal Handler from child to parent when EOF occured
void EndFile(int k)
{
    fend=1;
}

//Signal Handler from child to parent when it's job get completed
static void hdl (int sig, siginfo_t *siginfo, void *context)
{
	for(int cnt = 0;cnt<NumProcess;cnt++)
    {
        if(cid[cnt]==(siginfo->si_pid))
        {  
            stt[cnt]=0; 
            printf("De-active Child :%d\n\n",cid[cnt]);
        }
    }
}

//function to check passed string have all character numeric or not
int PropDig(char strng[])
{
    int strleng = strlen(strng);
    int i=0;
    while(i++<strleng-1)
    {
        if(!isdigit(strng[i]))
        {
            return 1;
        }   
    }
    return 0;
}

//main function
int main(int argn,char *argc[])
{
    //check if arguments are in correct form
    if(argn!=3)
    {
        printf("Please Enter <filename> <NumOfProcess>\n");
        exit(1);
    }

    if(PropDig(argc[2]))
    {
        printf("Please Enter valid number of child process\n");
        exit(1);
    }
    NumProcess = atoi(argc[2]);
    if(NumProcess<1)
    {
        printf("Number of child process should be greater than 1\n");
        exit(1);
    }
   
    //Arrays to maintain child id and their status
    cid = (int *)malloc(sizeof(int)*NumProcess);
    stt = (int *)malloc(sizeof(int)*NumProcess);

    int i = 0;
    //Buffer to read data to kernel buffer
    char *buff;
    int buflen=500;
    int flagInitial=1;

    //Array to count to which child is called how many times
    int TimesCalled[NumProcess];

    //Signal handlers
    signal(SIGUSR2,sig);
    signal(SIGSTOP,sig2);
    signal(SIGALRM,EndFile);

    struct sigaction act;
    memset (&act, '\0', sizeof(act));
    act.sa_sigaction = &hdl;
    act.sa_flags = SA_SIGINFO;

    if (sigaction(SIGQUIT, &act, NULL) < 0) {
		perror ("sigaction");
		return 1;
	}

    //Opening file
    fp = fopen(argc[1],"r");
    if(fp==NULL)
    {
        perror("Error while reading file : \n");
        exit(1);
    }

    fflush(fp);
    int bi;
    fseek(fp,0,SEEK_SET); 
    buff = (char*) malloc (buflen*sizeof(char));
    for(bi=0;bi<buflen;bi++)
        buff[bi]='\0';
    char c;

    //creating child
    for(i=0;i<NumProcess;i++)
    {
        stt[i]=0;
        TimesCalled[i]=0;
        if((cid[i]=fork())==0)
        {
            //Child process
            while(!end)
            {
                while(!fl);
                if(!end)
                {     
                    bi=0;
                    c = fgetc(fp);
                    while(c!='\n'&&bi<buflen)
                    {
                        if(c==-1)
                        {
                            printf("ENd Called By : %d\n",getpid());
                            kill(getppid(),SIGALRM);
                            break;
                        }
                        buff[bi++]=c;
                        c=fgetc(fp);
                    }
                    fflush(fp);
                    printf("Printing Child %d : %s\n",getpid(),buff);
                    memset(buff,0,strlen(buff));
                    sleep(1);
                    kill(getppid(),SIGQUIT);
                    fl=0;
                }
            }
            exit(0);
        }
        printf("%d Child Process: %d\n",i+1,cid[i]);
        // sleep(1);
    }


    //Round-Robin fashion allocating file
    while(!fend)
    {
        for(int bit =0;((bit<NumProcess)&&(!fend));bit++)
        {
            if(stt[bit]==0)
            {
                TimesCalled[bit]++;
                printf("Active Child : %d\n",cid[bit]);
                stt[bit]=1;
                kill(cid[bit],SIGUSR2);
            }
            sleep(1);
        }
    }

    //Process completed
    display();
    while(1);
    for(i=0;i<NumProcess;i++)
    {
        stt[i]=0;
        printf("%d->%d\n",cid[i],TimesCalled[i]);
        kill(cid[i],SIGINT);
    }
    display();
    fclose(fp);
    printf("Done\n");
    exit(0);

}