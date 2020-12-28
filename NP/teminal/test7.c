#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

void main()
{
    struct rlimit rl;
    int p[2];
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
    perror("can't get file limit :");
    
    //close fds
    if (rl.rlim_max == RLIM_INFINITY)
        rl.rlim_max = 1024;
    for (int i = 0; i < rl.rlim_max; i++)
        close(i);
    //signal dfl
    for (int i = 0; i < _NSIG; i++)
        signal(i,SIG_DFL);

    //mask signals
    sigset_t allsigs,old;
    sigfillset(&allsigs);
    sigprocmask(SIG_SETMASK,&allsigs,&old);

    pipe(p);

    if(fork==0)
    {
        //2nd child
        setsid();
        if(fork()==0)
        {

           int fd = open("/dev/null", O_RDWR);
           dup2(fd,0);
           dup2(fd,1);
           dup2(fd,2);

           umask(0);

            if (chdir("/") < 0)
                perror("%s: can't change directory to /");
            
            FILE *fp = fopen("run/ELdaemon.pid","w");
            char ch[10];
            sprintf(ch,"%d",getpid());
            fputs(ch,fp);
            fclose(fp);

            close(p[0]);
            write(p[1],"1",2);
            while(1);
        }
        else
        {
            close(p[0]);
            close(p[1]);
            exit(0);
        }
            
    }    
    else
    {
        char ch[10];
        close(p[1]);
        read(p[0],ch,2);
        exit(0);
        //1st child
    }

}
