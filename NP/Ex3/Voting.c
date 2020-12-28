#include<stdio.h>
#include<sys/msg.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#include<signal.h>
#include<time.h>

#define M_path "Voting.c"
int end=1;
int nchild, *strt;
struct my_msgbuf{
    long mytype;
    int pid;
    char mtext[200];
    int tp;
};

void sng(int d)
{
    for(int i=0;i<nchild;i++)
    {
        kill(SIGKILL,strt[i]);
    }
    end=0;
}

void main(int arg, char *argv[])
{
    if(arg!=2)
    {
        printf("Error : Please enter correct from of argument : ");
        exit(1);
    }
    nchild = atoi(argv[1]);
    int arr[nchild];
    strt = &arr[0];
    int msgquid1,msgquid2;
    struct my_msgbuf buf,buf2,buf3,buf4;
    printf("%d=-->\n",nchild);
    key_t key1,key2;
    if((key1=ftok(M_path,'D'))==-1)
    {
        perror("ftok");
        exit(0);
    }
    if((key2=ftok(M_path,'C'))==-1)
    {
        perror("ftok");
        exit(0);
    }

    if((msgquid1 = msgget(key1,IPC_CREAT|0x666))==-1)
    {
        perror("msgquid1 : ");
        exit(0);
    }
    if((msgquid2 = msgget(key2,IPC_CREAT|0x666))==-1)
    {
        perror("msgquid2 : ");
        exit(0);
    }

    for(int i=0;i<nchild;i++)
    {
        if((arr[i]=fork())==0)
        {
            int vote=-1;
            srand(time(NULL) - i*5);
            while(1)
            {
                memset(&buf,0,sizeof(buf));
                if(msgrcv(msgquid1,&buf,sizeof(buf),getpid(),0)==-1)
                {   
                     perror("msgrcv");
                     exit(0);
                }
                memset(&buf2,0,sizeof(buf2));
                vote = rand()%2;
                sprintf(buf2.mtext,"%d",vote);
                printf("Child %d Voted : %d\n",getpid(),vote);
                buf2.mytype=getpid();
                buf2.pid = getpid();
                if(msgsnd(msgquid2,&buf2,sizeof(buf2),0)==-1)
                {
                    perror("msgscd : ");
                }
            }
            exit(0);
        }
    }

    signal(SIGINT,sng);
        int num0=0,num1=0;
    printf("Child created : \n");
    while(end)
    {

        buf3.pid=1;
        for(int i=0;i<nchild;i++)
        {                
            memset(&buf3,0,sizeof(buf3));
            buf3.mytype = arr[i];
            sprintf(buf3.mtext,"start");
            buf3.pid =getpid();
            if(msgsnd(msgquid1,&buf3,sizeof(buf3),0)==-1)
            {   
                perror("msgsnd : ");
                exit(0);
            }
        }
        sleep(1);
        printf("All child are signaled :\n");
        num0=0;
        num1=0;
        for(int i=0;i<nchild;i++)
        {
            memset(&buf4,0,sizeof(buf4));
            if(msgrcv(msgquid2,&buf4,sizeof(buf4),arr[i],0)==-1)
            {
                perror("msgrcv : ");
                exit(0);
            }
            int k = atoi(buf4.mtext);
            if(k==0) num0++;
            else num1++;
        }

        if(num0>num1) printf("Major is 0:\n");
        if(num0<num1) printf("Major is 1:\n");
        if(num0==num1) printf("draw:\n");
        sleep(1);
    }
    printf("All child kills : \n");
    msgctl(msgquid1,IPC_RMID,NULL);
    msgctl(msgquid2,IPC_RMID,NULL);
    exit(0);
}