#include<stdio.h>
#include<sys/msg.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>

struct my_msgbuf{
    long mytype;
    int pid;
    char mtext[200];
};

int main()
{
    struct my_msgbuf buf;
    int msgquid;
    if((msgquid = msgget(12,IPC_CREAT))==-1)
    {
        perror("msgget : ");
    }
    printf("Server Ready to recevie msg :\n");
    buf.mytype=1;
    for(;;)
    {
        if(msgrcv(msgquid,&buf,sizeof(buf),1,0)==-1)
            perror("msgrcv");
        for(int i=0;i<strlen(buf.mtext);i++)
            buf.mtext[i]=toupper(buf.mtext[i]);
        buf.mytype = buf.pid;
        if(msgsnd(msgquid,&buf,sizeof(buf),0)==-1)
            perror("msgsnd : ");        
    }
    return 0;
}