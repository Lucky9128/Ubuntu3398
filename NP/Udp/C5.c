#include<stdio.h>
#include<string.h>
int MAX =15;

int main() 
{ 

    char abc2[100] = "Hello Brother \n How";


    char *ptr = strchr(abc2,'\n');
        if(ptr!=NULL)
        {
            *ptr = 0;
        }


    printf("%s\n",abc2);
  
    return 0; 
} 