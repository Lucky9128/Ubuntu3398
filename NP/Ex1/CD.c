#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <errno.h> 
#include <sys/wait.h> 
#include <signal.h>
#include<string.h>
 
static void hdl (int sig, siginfo_t *siginfo, void *context)
{
	printf ("Sending PID: %ld, UID: %ld\n",
			(long)siginfo->si_pid, (long)siginfo->si_uid);
}


int main (int argc, char *argv[])
{
	struct sigaction act;
 
	memset (&act, '\0', sizeof(act));
 
	/* Use the sa_sigaction field because the handles has two additional parameters */
	act.sa_sigaction = &hdl;
 
	/* The SA_SIGINFO flag tells sigaction() to use the sa_sigaction field, not sa_handler. */
	act.sa_flags = SA_SIGINFO;
 
	if (sigaction(SIGQUIT, &act, NULL) < 0) {
		perror ("sigaction");
		return 1;
	}
    sleep(4);
    printf("Called : %d\n",getpid());
    kill(getpid(),SIGQUIT);
	while (1)
		sleep (10);
    
	return 0;
}