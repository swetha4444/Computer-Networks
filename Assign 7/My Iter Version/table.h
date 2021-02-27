#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAXLINE 1024

//Named relative to local server
#define localCliPORT 4000
#define rootLocPORT 4002
#define tldRootPORT 4004 //here we only use .com TLD
#define authTldPORT 4006 //here we only use .com auth
#define authLocPORT 4008 //here we only use .com auth


struct Table
{
    char domainName[200];
    char ip[10]; //IP
};

typedef struct Table Table;


//find if a string matches to domainName of list of Tables of size k

int error(char *msg)
{
	perror(msg);
	exit(1);
}

int setConn(struct sockaddr_in *conn, int port, int is_bound, char *conn_name)
{
	printf("\nConnecting to %s",conn_name);
	int sockfd;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	if(sockfd < 0)
		error("Error in creating socket.\n");
		
	bzero(conn, 16);
	conn->sin_family = AF_INET;
	conn->sin_addr.s_addr = inet_addr("127.0.0.1");
	conn->sin_port = htons(port);

	if(is_bound)
		if(bind(sockfd, (struct sockaddr *)conn, 16) < 0)
			error("Error in binding.\n");
    	else
	    	printf("\nConnection to %s is successful.\n", conn_name);

	return sockfd;
}

int checkTable(Table lookup[50], char *hostname , int k) 
{
	for(int i = 0; i < k; i++)
		if(strcmp(hostname, lookup[i].domainName) == 0)
	    	  return i;

	return -1;
}
