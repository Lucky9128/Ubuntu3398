#include "stdio.h"
#include "string.h"

int main()
{
    char ch2[10]={0};
    char ch[4] = "luck";
    char *c = &ch[0];
    strcpy(ch2,c+2);
    printf("%s\n",ch2);
}