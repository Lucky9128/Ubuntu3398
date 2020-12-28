#include<stdio.h>
#include<sys/msg.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
int max =1000000;
struct my_msgbuf{
    long mtype;
    int pid;
    char mtext[1000000];
};
int main(int arg, char *argv[])
{
    if(atoi(argv[1])==1)
    {//only sending
        struct my_msgbuf buf;
        int pid = argv[2];
        int msqid;
        if((msqid=msgget(pid,0))==-1)
        {
            perror("Got error in msquid:\n");
        }
        char buff[1000000]={0};
        char c;
        int i=0;
        while(1)
        {
            i=0;
            c=getc(stdin);
            while(c!='\0'||c!='\n')
            {
                buff[i++]=c;
            }
            buff[i]='\0';
            buf.mtype = 1;
            strcpy(buf.mtext,buff);
            if(msgsnd(msqid,&buf,i+2,0)==-1)
            {
                perror("msgsnd\n");
            }
        }
    }
    // else if(atoi(argv[1])==2)
    // {
    //     int p1,p2;
    //     if((p1=fork())==0)
    //     {//sending
    //         struct my_msgbuf buf;
    //         int pid = atoi(argv[2]);
    //         int msqid;
    //         if((msqid=msgget(pid,0))==-1)
    //         {
    //             perror("Got error in msquid:\n");
    //         }
    //         int p[2];
    //         pipe(p);
    //         close(p[1]);
    //         dup2(p[0],0);
    //         char buff[1000000]={0};
    //         char c;
    //         int i=0;
    //         while(1)
    //         {
    //             i=0;
    //             c=getc(stdin);
    //             while(c!='\0'||c!='\n')
    //             {
    //                 buff[i++]=c;
    //             }
    //             buff[i]='\0';
    //             buf.mtype = pid;
    //             strcpy(buf.mtext,buff);
    //             if(msgsnd(msqid,&buf,i+2,0)==-1)
    //             {
    //                 perror("msgsnd\n");
    //             }
    //         }
    //         exit(0);
    //     }
    //     else if((p2=fork())==0)
    //     {//receiving
    //         struct my_msgbuf buf;
    //         int pid = atoi(argv[3]);
    //         int msqid;
    //         if((msqid=msgget(pid,0))==-1)
    //         {
    //             perror("Got error in msquid:\n");
    //         }
    //         char buff[]={0};
    //         char c;
    //         int i=0;
    //         while(1)
    //         {
    //             bzero(buff,max);
    //             buf.mtype = 1;
    //             strcpy(buf.mtext,buff);
    //             if(msgrcv(msqid,&buf,max,1,0)==-1)
    //             {
    //                 perror("msgsnd\n");
    //             }
    //             printf("%s",buff);
    //         }
    //         exit(0);
    //     }
    // }
    else if(atoi(argv[1])==2)
    {
        struct my_msgbuf buf;
        int pid = atoi(argv[2]);
        int msqid;
        if((msqid=msgget(pid,0))==-1)
        {
            perror("Got error in msquid:\n");
        }
        char buff[]={0};
        char c;
        int i=0;
        while(1)
        {
            bzero(buff,max);
            buf.mtype = 1;
            strcpy(buf.mtext,buff);
            if(msgrcv(msqid,&buf,max,1,0)==-1)
            {
                perror("msgsnd\n");
            }
            printf("%s",buff);
        }
        exit(0);
    }



    return 0;
}