// Client side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define PORT	 8080 
#define MAXLINE 1024 

// Driver code 
int main() { 
	int sockfd; 
	char buffer[MAXLINE]; 
	char hello[MAXLINE];
	struct sockaddr_in	 servaddr; 

	

    int port2;
    char chr[5];

    while(1)
    {
        fputs("Enter port : ",stdout);
        fgets(chr,6,stdin);
        fputs("Enter cmd : ",stdout);
        fgets(hello,MAXLINE,stdin);
        
        char *ptr = strchr(hello,'\n');
        if(ptr!=NULL)
        {
            *ptr = 0;
        }

        port2 = atoi(chr);
        // Creating socket file descriptor 

        if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
            perror("socket creation failed"); 
            exit(EXIT_FAILURE); 
        } 
        memset(&servaddr, 0, sizeof(servaddr)); 
        // Filling server information 
        servaddr.sin_family = AF_INET; 
        servaddr.sin_port = htons(port2); 
        servaddr.sin_addr.s_addr = INADDR_ANY; 
        int n, len; 
        
        sendto(sockfd, (const char *)hello, MAXLINE, 
            MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
                sizeof(servaddr)); 
            
        n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
                    MSG_WAITALL, (struct sockaddr *) &servaddr, 
                    &len); 
        buffer[n] = '\0'; 
        printf("Server : %s\n", buffer); 
        memset(chr,0,5); 
        memset(hello,0,MAXLINE);
        close(sockfd); 
    }
        
	return 0; 
} 
