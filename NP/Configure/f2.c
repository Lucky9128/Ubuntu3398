#include <stdio.h>
#include <stdlib.h>
#include <string.h>



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


int main(char argc,char *argv[]) 
{
    FILE *fp = fopen("config.file","r");
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
    port = ReturnPort(root,argv[1]);
    if(port==-1)
    {
        printf("No such node found ..\n");
        exit(0);
    }
    else
    {
        printf("Conencting at %d ..\n",port);
    }
    



}