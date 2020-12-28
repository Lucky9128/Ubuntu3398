#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include<string.h>
#include<stdlib.h>

#define MSGSIZE 16
void clear(char *ab)
{
  for(int i =0;i<MSGSIZE;i++)
    *(ab+i)=0;
}
void sigh(int ch)
{
}
main ()
{
  int i;
  char *msg = "How are you?";
  char inbuff[MSGSIZE];
  sigset_t new,old;
  int p[2];
  pid_t ret;
  pipe (p);
  ret = fork ();
  signal(SIGUSR1,sigh);
  sigemptyset(&new);
  sigemptyset(&old);
  sigaddset(&new,SIGUSR1);
  sigprocmask(SIG_BLOCK,&new,NULL);
  clear(&inbuff[0]);
  if (ret == 0)
    {
      i = 0;
      while (i < 10)
        {
          write (p[1], msg, MSGSIZE); 
          kill(getppid(),SIGUSR1);  
          sigsuspend(&old);       
          // printf("C\n");
          read (p[0], inbuff, MSGSIZE);   
          // printf("C\n");    
          printf("hello\n");
          printf ("Parent: %s\n", inbuff);
          i++;
          clear(&inbuff[0]);
        }
    exit(1);
    }
  else
    {
      i = 0;
      while (i < 10)
        {
          sigsuspend(&old);
          // printf("P\n");
          read (p[0], inbuff, MSGSIZE);
          // printf("P\n");
          printf ("Child: %s\n", inbuff);
          // printf("P\n");
          write (p[1], "i am fine", strlen ("i am fine"));
          // printf("P\n");
          kill(ret,SIGUSR1);
          // printf("P\n");
          i++;
          clear(&inbuff[0]);
        }
    }
  exit (0);
}
