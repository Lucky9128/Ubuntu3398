#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <errno.h> 
#include <sys/wait.h> 
#include <string.h>

int cid[5],stt[5],file_read=1;
int sig(int k)
{
    printf("Child %d\n",getpid());
    sleep(1);
    kill(getppid(),SIGQUIT);
    sleep(1);
    kill(getpid(),SIGSTOP);
}
 
static void hdl (int sig, siginfo_t *siginfo, void *context)
{
	for(int k=0;k<5;k++)
    {
        if(cid[k]==((int)siginfo->si_pid))
            stt[k]=0;
    }
}


int main()
{
    int i=0;
    struct sigaction act;
    memset (&act, '\0', sizeof(act));
    act.sa_sigaction = &hdl;
    act.sa_flags = SA_SIGINFO;

    if (sigaction(SIGQUIT, &act, NULL) < 0) {
		perror ("sigaction");
		return 1;
	}

    signal(SIGCONT,sig);
    printf("\t\t\t\t  Pid\n");
    for(i=0;i<5;i++)
    {
        stt[i]=0;
        if((cid[i]=fork())==0)
        {
            // sleep(1);
            printf("%d Child Process: %d\n",i+1,getpid());
            kill(getpid(),SIGSTOP);
        }
        sleep(1);
    }
    // sleep(1);

    printf("%d\n",getpid());
    sleep(1);
    stt[1]=1;
    stt[3]=1;
    stt[4]=1;
    kill(cid[1],SIGCONT);
    // sleep(2);
    kill(cid[3],SIGCONT);
    sleep(5);
    for(i=0;i<5;i++)
    {
        printf("%d -> %d\n",cid[i],stt[i]);
    }
    sleep(1);
    for(i=0;i<5;i++)
    {
        kill(cid[i],SIGINT);
    }
    // sleep(2);
}