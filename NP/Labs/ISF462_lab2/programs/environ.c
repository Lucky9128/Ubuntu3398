#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
  int i;
  char **ptr;
  extern char **environ;

  for (i = 0; i < argc; i++)    /* echo all command-line args */
    printf ("argv[%d]: %s\n", i, argv[i]);

  setenv("Gende","Male",1);
  putenv("NAME=lucky");

  for (ptr = environ; *ptr != 0; ptr++) /* and all env strings */
    printf ("%s\n", *ptr);
 printf ("-------------------------------------------------------------------------------------------------------------------------------\n");
  unsetenv("Gende");
  for (ptr = environ; *ptr != 0; ptr++) /* and all env strings */
    printf ("%s\n", *ptr);

  exit (0);
}
