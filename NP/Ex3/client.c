#include<stdio.h>
#include<sys/msg.h>
#include<unistd.h>


struct my_msgbuf{
    long mytype;
    int pid;
    char mtext[200];
};

int main()
{
    struct my_msgbuf buf,buf2;
    int msgquid;
    if((msgquid = msgget(12,IPC_CREAT))==-1)
    {
        perror("msgget :");
    }
    printf("Enter Text :");
    while(gets(buf.mtext),!feof(stdin))
    {
    buf.mytype=1;
    buf.pid = getpid();
        if(msgsnd(msgquid,&buf,sizeof(buf),0)==-1)
            perror("msgsnd: ");
        if(msgrcv(msgquid,&buf2,sizeof(buf2),getpid(),0)==-1)
            perror("msgrcv");
        printf("Message received : %s\n",buf2.mtext);
    }
    return 0;
}