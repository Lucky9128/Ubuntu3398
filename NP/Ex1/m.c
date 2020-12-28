#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>

char *buf;
int buflen;
FILE *fp ;

int siig(int k)
{
    getline(&buf,buflen,fp)
    printf("")
}

void main(int argn, char *argv[])
{
    int i=0 ;
    fp =signal(SIGCONT,siig);  
    int num_proc = atoi(argv[1]);
    int cid[5]; 
    = fopen("test.txt","r");

    for(i=0;i<5;i++)
    {
        if((cid[i]=fork())==0)
        {
            printf("Child created %d\n",getpid());
            while(1);
        }
        sleep(1);
    }
    sleep(3);
    printf("Child id's:\n");

        // printf("%d -> %d\n",i,cid[0]);
        kill(cid[0],SIGCONT);
        sleep(2);
        kill(cid[0],SIGINT);
    
    sleep(4);
    // for(i=0;i<5;i++)
    // {
    //     kill(cid,SIGINT);
    // }
    // sleep(1);
}