#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

main ()
{
  int i = 0, j = 0; 
  pid_t ret;
  int status;

  ret = fork ();
  if (ret == 0)
    {
      for (i = 0; i < 100; i++)
        printf ("Child: %d\n", i);
      printf ("Child ends\n");
    }
  else
    {
      waitpid(ret,&status,WNOHANG);
      printf ("Parent resumes.\n");
      for (j = 0; j < 100; j++)
        printf ("Parent: %d\n", j);
    }
}
