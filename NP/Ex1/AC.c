#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <string.h>
#include <errno.h> 
#include <sys/wait.h> 
int fl = 1;
int end = 1,fend=1,all=1;
int cid[5],stt[5],file_read=1;
FILE *fp;

void sig(int k)
{
    fl=0;    
}

int sig2(int k)
{
    end=0;    
    fl=0;
}
int EndFile(int k)
{
    printf("File Ended");
    fend=0;
}
static void hdl (int sig, siginfo_t *siginfo, void *context)
{
	for(int cnt = 0;cnt<5;cnt++)
    {
        // printf("Checking .. %d -> %d\n",cid[cnt],siginfo->si_pid);
        // sleep(1);
        if(cid[cnt]==(siginfo->si_pid))
        {
            // printf("Enter\n");
            stt[cnt]=0;
        printf("DeAllocating Child : %d\n",cid[cnt]);
        }

        // sleep(1);
    }
}
void display()
{
    printf("Current Status : [");
    for(int i=0;i<5;i++)
    {
        printf("%d ",stt[i]);
    }
    printf("]\n");
    
}
int main()
{
    int i = 0;
    char *buff;
    int buflen=500;


    signal(SIGCONT,sig);
    signal(SIGSTOP,sig2);
    signal(SIGALRM,EndFile);
    struct sigaction act;
    memset (&act, '\0', sizeof(act));
    act.sa_sigaction = &hdl;
    act.sa_flags = SA_SIGINFO;

    if (sigaction(SIGUSR2, &act, NULL) < 0) {
		perror ("sigaction");
		return 1;
	}



    fp = fopen("map2.txt","r");
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

    for(i=0;i<5;i++)
    {
        stt[i]=0;
        if((cid[i]=fork())==0)
        {
            // sleep(1);
            printf("%d Child Process: %d\n",i+1,getpid());
            while(end)
            {
                while(fl);
                if(end)
                {     
                    bi=0;
                    printf("Reading Child : %d ->",getpid());
                    c = fgetc(fp);
                    while(c!='\n'&&bi<buflen)
                    {
                        if(c==-1)
                        {
                            // kill(getppid(),SIGALRM);
                            break;
                        }
                        buff[bi++]=c;
                        c=fgetc(fp);
                    }
                    fflush(fp);
                    printf(": %s\n",buff);
                    while(bi--)
                        buff[bi]='\0';
                    kill(getppid(),SIGUSR2);
                    fl=1;
                }
            }
            exit(0);
        }
        sleep(1);
    }
    sleep(1);
    // printf("%d\n",getpid());
    // while(fend)
    // {
        for(int bit =0;(bit<5);bit++)
        {
            if(stt[bit]==0)
            {
                // sleep(1);
                printf("Allocating Child : %d\n",cid[bit]);
                stt[bit]=1;
                kill(cid[bit],SIGCONT);
            }
        }
        sleep(10);
        // while(all);
    // }
    sleep(1);
    display();
    for(i=0;i<5;i++)
    {
        kill(cid[i],SIGINT);
    }
    sleep(4);
    display();
    fclose(fp);
    exit(0);
}
