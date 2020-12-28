#include<stdio.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include <errno.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <fcntl.h>
#include <arpa/inet.h> 
#define MAX 100000 
#define SA struct sockaddr 

char ConFile[500] = "config.file";
void func(char *buff,int sockfd, char *output) 
{ 
    int n; 
    //for (;;) {
		strcat(buff,"\0");
		for(n=0;buff[n];n++)
			;
		// printf("write %d characters\n",n);
		strcat(buff,"\n");
        write(sockfd, buff, n); 
        bzero(output, MAX);
		if ((strncmp(buff, "exit", 4)) == 0) { 
            sprintf(output,"Client Exit ..");
            return;
        } 
		// printf("read\n");
        read(sockfd, output, MAX);
        return;
    //} 
} 

//Structure for node---------//
struct node 
{
    int port;
    char name[20];
    struct node *next;
};
int ReturnPort(struct node *temp,char name1[20])
{
    int port=-1;
    while(temp!=NULL)
    {
        if(strcmp(temp->name,name1)==0)
        {
            port = temp->port;
            break;
        }
        else
        {
        temp = temp->next;
        }
    }
    return port;
}

struct node * AddData(struct node* root,int port,char name[20])
{
    struct node *n1 = (struct node *)malloc(sizeof(struct node));

    strcpy(n1->name,name);
    n1->port = port;
    n1->next = NULL;

    struct node *temp = root;

    if(temp==NULL)
    {
        temp = n1;
        return temp;
    }   
    else
    {
        while(temp->next!=NULL)
        {
            temp=temp->next;
        }
        temp->next = n1;
     
        return root;
    }
}



void client(char *buff,int port,char *output)
{
    
    //--------------------client connection ---------------------//

    int sockfd, connfd; 
    struct sockaddr_in servaddr, cli; 
    bzero(output,MAX);
    // socket create and varification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        sprintf(output,"socket creation failed...\n"); 
        return; 
    }  
    bzero(&servaddr, sizeof(servaddr)); 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(port); 
  
   
    
    // connect the client socket to server socket 
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
        sprintf(output,"connection with the server failed...\n"); 
    } 

    // function for chat 
    func(buff,sockfd,output); 
  
    // close the socket 
    close(sockfd); 
} 
struct node * CreatePortFile(char *addr)
{
    FILE *fp = fopen(addr,"r");
    if(fp==NULL)
    {
        perror("File opening Error : ");
        exit(0);
    }
    struct node *root = NULL;
    int port;
    char name[20];
    int i=0;
    int flag=0;
    int j=0;
    while(!feof(fp))
    {   
        memset(name,0,20);
        port=0;
        fscanf(fp,"%s %d\n",name,&port);  
        if(port!=0)
        {
            root=AddData(root,port,name);
        }
        // printf("***********\n");
    }   
    fclose(fp);
    return root;
}

void client2(char *output,char *rescmd,char *input) 
{
    struct node *root  = CreatePortFile(ConFile);
    int port=-1,res;
    char FinalOutput[MAX];
    char *c;
    bzero(output,MAX);
    if((c=strchr(rescmd,'.'))!=NULL)
    {
        *c++='\0';
        char command[1000];
        int i;
        for(i=0;*(c+i);i++)
        {
            command[i]=*(c+i);
            *(c+i)='\0';
        }
        command[i]='\0';

        port = ReturnPort(root,rescmd);
        if(port==-1)
        {
            sprintf(output,"Node %s not active..",rescmd);
            return;
        }
        else
        {
            char NewCmd[MAX]={0};
            strcpy(NewCmd,input);
            strcat(NewCmd,"|");
            strcat(NewCmd,command);
            client(NewCmd,port,output);
        }
        return;
    }
    else
    {
        sprintf(output,"Write instruction in proper format");
        return;
    }
    
}

int main(char argc,char *argv[]) 
{
    char Out[MAX]={0};
    char rescmd[MAX] = "n2.wc";
    char input[MAX] = "output:Hello brother how are you";
    client2(&Out[0],&rescmd[0],&input[0]);   
}


