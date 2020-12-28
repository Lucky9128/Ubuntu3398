#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int arg, char *argv[]) {
    if(arg!=2)
    {
        printf("Please enter node name as cmd argument:\n");
        exit(0);
    }
    FILE *fp = fopen("config.file","r");
    if(fp==NULL)
    {
        perror("File opening Error : ");
        exit(0);
    }

    char port[6];
    char name[20];
    char c = fgetc(fp);
    int i=0;
    int flag=0;
    while(c!=-1)
    {   
        memset(name,0,20);
        memset(port,0,5);
        i=0;
        while(c!=32)
        {
            name[i++]=c;
            c=fgetc(fp);
        }
        // printf("name -> %s\n",name);
        i=0;
        c=fgetc(fp);
        while(c!='\n')
        {
            port[i++]=c;
            c=fgetc(fp);
        }
        // printf("port ->%s\n",port);
        c=fgetc(fp);

        if(strcmp(name,argv[1])==0)
        {
            flag=1;
            break;
        }
    }
    fclose(fp);
    if(!flag)
    {
        printf("No such file found please check config file ...\n");
    }


    
}