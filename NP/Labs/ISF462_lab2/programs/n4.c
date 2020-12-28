#include <stdio.h>
#include <sys/wait.h>
int kb=1;

static void Sig(int k)
{
    printf("Stop is call\n");
}
static void Sig2(int k)
{
    printf("conti is call\n");
    
}
static void SiCnt(int k)
{
    kb=2;
}

 int main()
 {
     int cid = fork();
    
 //   signal(SIGCONT,SiCnt);
  //  signal(SIGSTOP,Sig);   
    signal(SIGINT,Sig2);   
    printf("%d\n",cid);
     if(cid==0)
     {
         while(kb==1)
            printf("helo\n");
     }
     else
     {
         sleep(1);
        kill(cid,SIGSTOP);
        sleep(2);
        kill(cid,SIGCONT);
        sleep(2);
        kill(cid,SIGINT);
        printf("Process ended:");
     }
     return 0;
 }
