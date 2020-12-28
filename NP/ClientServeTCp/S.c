// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h> 
#define PORT 8080 
int main(int argc, char const *argv[]) 
{ 
	int server_fd, new_socket, valread; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 
	char buffer[1024] = {0}; 
	char hello[1000] = {0};

	FILE *fp = fopen("N1.txt","w");
	fprintf(fp,"%c%c%c",'1','\n','2');
	fclose(fp);
 	//printf("%d %d %d %d %d %d\n",hello[0],hello[1],hello[2],hello[3],hello[4],hello[5]);
	

	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	// Forcefully attaching socket to the port 8080 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	inet_aton("127.0.0.1",&address.sin_addr); 
	address.sin_port = htons( PORT ); 
	
	// Forcefully attaching socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&address, 
								sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (listen(server_fd, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
					(socklen_t*)&addrlen))<0) 
	{ 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	} 
	FILE *fp2 = fopen("N1.txt","r");
	setvbuf(fp2,&hello[0],_IOLBF,1000);	
	int k = fread(&hello[0],1,1000,fp2);
 	printf("%d %d %d %d %d %d %d\n",k,hello[0],hello[1],hello[2],hello[3],hello[4],hello[5]);
	fclose(fp2);
	valread = read( new_socket , buffer, 1024); 
	printf("%s\n",buffer); 
	int sended = send(new_socket , hello , strlen(hello) , 0 ); 
    	printf("Sending %s ->%d\n",hello,sended);
	//printf("Hello message sent\n"); 
	return 0; 
} 
