
// C program to demonstrate working of strtok_r() 
// by splitting string based on space character. 
#include <stdio.h> 
#include <string.h> 
  int pro1() 
{ 
    char str[] = "Geeks-for-Geeks"; 
  
    // Returns first token 
    char* token = strtok(str, "-"); 
  
    // Keep printing tokens while one of the 
    // delimiters present in str[]. 
    while (token != NULL) { 
        printf("%s\n", token); 
        token = strtok(NULL, "-"); 
    } 
  
    return 0; 
} 
int pro2() 
{ 
    char str[] = "Geeks for Geeks"; 
    char* token; 
    char* rest = str; 
  
    while ((token = strtok_r(rest, " ", &rest))) 
        printf("%s\n", token); 
  
    return (0); 
} 

int main()
{
    pro1();
    pro2();
}