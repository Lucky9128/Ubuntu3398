#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <errno.h> 
#include <sys/wait.h> 
#include <string.h>

int sig(int k)
{
    printf("Child %d\n",getpid());
    kill(getppid(),SIGQUIT);
    sleep(1);
    kill(getpid(),SIGSTOP);
}
 
static void hdl (int sig, siginfo_t *siginfo, void *context)
{
	printf ("Sending PID: %ld, UID: %d\n",
			(long)siginfo->si_pid, getpid());
}


int main()
{
    int i = 0,cid[5],stt[5];
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
    for(int i=0;i<5;i++)
    {
        if(i%2==0)
            {
                sleep(1);
                kill(cid[i],SIGCONT);
                stt[i]=1;
            }
        // sleep(1); 
    }
    sleep(5);
    for(int i=0;i<5;i++)
    {
        printf("%d",stt[i]);
        // kill(cid[i],SIGCONT);
        // sleep(1); 
    }
    printf("\n");
    // for(i=0;i<5;i++)
    // {
    //     printf("CId %d\n",cid[i]);
    // }
    // for(i=0;i<5;i++)
    // {
    //     kill(cid[i],SIGCONT);
    //     // sleep(3);
    // }
    sleep(1);
    for(i=0;i<5;i++)
    {
        kill(cid[i],SIGINT);
    }
    // sleep(2);
}