#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define MAXLINE 1024

int main(int argc, char **argv)
{
	char buffer[MAXLINE];
	char hostname[200] ;
	char temp[100];
	int port = atoi(argv[1]);
	char cont;
	int sockfd, n , op;
	struct sockaddr_in serveraddr;
	socklen_t len;

	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serveraddr.sin_port = htons(port);
	serveraddr.sin_family = AF_INET;

	len = sizeof(serveraddr);

	sockfd = socket(PF_INET, SOCK_DGRAM, 0);

	if(connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0)
	{
       	printf("\nConnect error!!!");
       	exit(1);
	}

	while(1)
	{
		bzero(buffer,MAXLINE);
		printf("\nEnter the host name:");
    		scanf(" %[^\n]", hostname);
    		strcpy(buffer,hostname);
    		sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&serveraddr, len);

    		recvfrom(sockfd, buffer, MAXLINE, 0, (struct sockaddr*)&serveraddr, &len);

    		strcpy(temp, buffer);
    		printf("\nIP ADDRESS:\n");

    		const char s[2] = ",";
    		char *token = strtok(temp,s);
    		while(token != NULL)
    		{
    			printf(" %s\n", token);
    			token = strtok(NULL,s);
		}


	}

	close(sockfd);
	return 0;
}
/*OUTPUT:
URL received : www.google.com
IP Addresses sent to client... : 216.58.193.68,
URL received : www.annauniv.edu
IP Addresses sent to client... : 14.139.161.7,
URL received : www.yahoo.com
IP Addresses sent to client... : 98.137.11.163,98.137.11.164,
URL received : www.google.in
IP Addresses sent to client... : 172.217.14.195,
URL received : www.airtel.in
IP Addresses sent to client... : 23.194.212.104,23.194.212.120,
URL received : www.amazon.in
*/
/*CKIENT 1

~/ $ ./c1 5004

Enter the host name:www.google.com

IP ADDRESS:
 216.58.193.68

Enter the host name:www.yahoo.com

IP ADDRESS:
 98.137.11.163
 98.137.11.164

Enter the host name:www.annauniv.edu

IP ADDRESS:
 14.139.161.7
*/

/*CLIENT 2:
~/ $ ./c2 5004

Enter the host name:www.annauniv.edu

IP ADDRESS:
 14.139.161.7

Enter the host name:www.google.in

IP ADDRESS:
 172.217.14.195

Enter the host name:www.airtel.in

IP ADDRESS:
 23.194.212.104
 23.194.212.120

Enter the host name:www.amazon.in

IP ADDRESS:
 184.26.81.59
*/