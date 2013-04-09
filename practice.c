#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

/*----------------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
   fork();
   fork();
   fork();
   fork();

   printf("children\n");

   int s;
   pid_t child;
   while(child != -1)
   {
      child = waitpid(-1, &s, 0); // or wait(&s);
   }
   exit(0);
}