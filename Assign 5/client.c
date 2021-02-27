#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SA struct sockaddr
#define SERVER_PORT 8000
#define MAX 256

int error(char *msg){
	perror(msg);
	exit(1);
}

int main(int argc, char const *argv[])
{
	int sockfd,n;
        int con = 1;
        char opt;
	struct sockaddr_in servaddr;
	char buf[1024];

	//CREATE SOCKET
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) <= 0) 			
		error("\033[0;31mSOCKET ERROR\033[0m");

	//CLEAR servaddr 
	bzero(&servaddr , sizeof(servaddr));
	
	// SET IP,PORT,FAMILY OF SERVER
	servaddr.sin_port = htons(SERVER_PORT);
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;

	//SEND REQUEST
	printf("\nRequest to Server:\n\t[1] Date\n\t[2] Day\n\t[3] Month\n\t[4] Year\n\t[5] Time\n\t[6] Toronto Time\nYour Option: ");
	scanf("%c", &opt);
	buf[0] = opt;
	sendto(sockfd, buf, sizeof(buf), 0, (SA*)&servaddr,sizeof(servaddr));
	bzero(buf,sizeof(MAX));
	
	//RECIEVE INFO 
	printf("\nRequesting information from server\n\n");
	int addrlen = sizeof(servaddr);
	recvfrom(sockfd, buf, sizeof(buf), 0, (SA*)&servaddr,&addrlen);
	printf("\nRecieved from server: %s\n",buf);
		
	return 0;
}


