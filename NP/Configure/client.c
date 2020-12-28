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
int func(char *buff,int sockfd) 
{ 
    int n; 
    //for (;;) {
		strcat(buff,"\0");
		for(n=0;buff[n];n++)
			;
		// printf("write %d characters\n",n);
		strcat(buff,"\n");
        write(sockfd, buff, n); 
		if ((strncmp(buff, "exit", 4)) == 0) { 
            printf("Client Exit...\n"); 
            return 1;
        } 
        char buf[100000];
        bzero(buf, sizeof(buf));
		// printf("read\n");
        read(sockfd, buf, sizeof(buf)); 
        printf("From Server : %s", buf); 
        return 0;
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

int client(char *buff,int port)
{
    
    //--------------------client connection ---------------------//

    int sockfd, connfd; 
    struct sockaddr_in servaddr, cli; 
  
    // socket create and varification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        return 3; 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(port); 
  
   
    
    // connect the client socket to server socket 
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection with the server failed...\n"); 
         //return 0 as connection failed for active command
        if(strcmp(buff,"active")==0)
        {   
            close(sockfd);
            return 0;
        }
        return 2; 
    } 
    else
        printf("connected to the server..\n"); 
    //return 1 as connection success for active command
    if(strcmp(buff,"active")==0)
    {   
        close(sockfd);
        return 1;
    }


    // function for chat 
    int ret = func(buff,sockfd); 
  
    // close the socket 
    close(sockfd); 
    return ret;
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

void PrintActive(struct node *temp)
{
    char cmd[6]="active";
    int port;
    while(temp!=NULL)
    {
        port=temp->port;
        if(client(cmd,port))
        {
            printf("Node %s is Active on port number : %d \n",temp->name,temp->port);
        }
        else
        {
            printf("Node %s is not Active \n",temp->name);
        }
        
        temp = temp->next;
    }
}

int main(char argc,char *argv[]) 
{
    struct node *root  = CreatePortFile(argv[1]);
    int port=-1,res;
    while(1)
    {
        char buff[MAX];
        bzero(buff, sizeof(buff)); 
        printf("Enter the string : "); 
        fgets(buff,sizeof(buff),stdin);
        char *c;
        if(strncmp(buff,"quit",4)==0||strncmp(buff,"Quit",4)==0||strncmp(buff,"Exit",4)==0||strncmp(buff,"EXIT",4)==0||strncmp(buff,"exit",4)==0||strncmp(buff,"QUIT",4)==0)
        {
            printf("exiting\n");
            exit(0);
            return 1;
        }
        if(strncmp(buff,"active",6)==0)
        {
            PrintActive(root);
            return 1;
        }
        if((c=strchr(buff,'.'))!=NULL)
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
	
            port = ReturnPort(root,buff);
            if(port==-1)
            {
                printf("No such node active...\n");
            }
            else
            {
               res = client(command,ReturnPort(root,buff));
               switch (res)
               {
               case 0:
                  continue;
                  break;
               case 1:
                    printf("Client Exit...\n");
                   exit(0);
                   break;
               case 2:
                    printf("Server not active at  %d...\n",port);
                   break;
               case 3:
                    printf("Error while creating socket Exit...\n");
                   break;
               
               default:
                   break;
               }
            }
        }
    }
}