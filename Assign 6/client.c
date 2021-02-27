#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> 
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>

#define SA struct sockaddr 
#define SERVER_PORT 4000 
#define MAX 256 

int error(char *msg)
{
	perror(msg);
	exit(1);
}

int main()
{
	int sock_fd = 0,flag = 0;
	struct sockaddr_in servaddr; 
	//PACKET
	int n;
	char buf[MAX];
	//IP & MAC
	char IP[MAX];
    char MAC[MAX];

	//CREATE SOCKET
	if ((sock_fd = socket(AF_INET,SOCK_STREAM,0)) <= 0) 
        error("SOCKET FAILED");

	//RESET servaddr 
	bzero(&servaddr, sizeof(servaddr)); 

    // ASSIGN IP, PORT, FAMILY  OF SERVER
	servaddr.sin_port = htons(SERVER_PORT); 
	servaddr.sin_family = AF_INET; 

	//CONNECT
	if ((connect(sock_fd, (SA*)&servaddr, sizeof(servaddr))) != 0) 
        error("CONNECT ERROR");

	printf("\nEnter IP address\t: "); //Enter IP
	scanf("%s",IP);
	write(sock_fd, IP, sizeof(IP));

	printf("\nEnter MAC address\t: "); //Enter MAC
	scanf("%s",MAC);
	write(sock_fd, MAC, sizeof(MAC));

	//Recieve ARP broadcast packet
	bzero(buf, MAX);
	read(sock_fd, buf, MAX);
	printf("\nARP Request Received\t: %s",buf);
	
	//Recieve dstIP-check and send MAC
	bzero(buf, MAX);
	read(sock_fd, buf, MAX);
	if(strcmp(IP,buf) == 0)
	{ 
		flag =1;
		printf("\nIP address matches.");
		write(sock_fd, MAC, sizeof(MAC));;
	}
	else
	{
		printf("\nIP address does not match.\n");
	}
	
	//Read data if dst
	if(flag == 1)
	{
		bzero(buf, MAX);
		read(sock_fd, buf, MAX);
		printf("\nReceived Packet is\t: %s\n",buf);
	}


    //CLOSE SOCKET
	close(sock_fd);
	return 0;
}
