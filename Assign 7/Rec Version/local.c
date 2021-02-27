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
#include "table.h"

#define MAXLINE 1024
Table lookup[30];

int main(int argc, char **argv)
{
    int port = atoi(argv[1]);       //user inputs the port for the local server
    int k = 0;
    int sockfd, tldfd, udpfd;
    char buffer[MAXLINE];
    socklen_t addr_size, len;
    fd_set readfds;
    struct sockaddr_in serverAddr, tldAddr, client;

    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if(sockfd < 0)
    {
        printf("\nError in creating socket!!");
        exit(1);
    }

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(port);
    serverAddr.sin_family = AF_INET;

    bzero(&tldAddr, sizeof(tldAddr));                    //establishing connection to the tld server
    tldAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    tldAddr.sin_port = htons(55621);
    serverAddr.sin_family = AF_INET;

    if(bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0)
    {
         printf("\nSocket BInd Failed");
         exit(1);
    }

    tldfd = socket(PF_INET, SOCK_DGRAM, 0);    //call to socket
    len = sizeof(tldAddr);

    if (connect(tldfd, (struct sockaddr*)&tldAddr, sizeof(tldAddr)) < 0)
    {
       printf("\nConnect error!!!");
       exit(1);
    }


    addr_size = sizeof(client);

    while(1)
    {
    	FD_SET(sockfd, &readfds);

    	int activity = select(sockfd + 1, &readfds, NULL, NULL, &tv);

    	if(activity < 0)
        {
             printf("Error in select()");
             exit(EXIT_FAILURE);
	}


	if(FD_ISSET(sockfd, &readfds))
	{
		addr_size = sizeof(client);
		recvfrom(sockfd, buffer, MAXLINE, 0, (struct sockaddr*)&client, &addr_size);

		char hostname[1024];
		strcpy(hostname, buffer);
		bzero(buffer, MAXLINE);
		int position = checkTable(lookup, hostname, k);             //if the domain is present in this buffer

		if (position == -1)                    //if not
		{
			sendto(tldfd, hostname, sizeof(hostname), 0, (struct sockaddr*) &tldAddr, len);  //pass it along the tld server

			strcpy(lookup[k].domainName,hostname);
			recvfrom(tldfd, buffer, MAXLINE, 0, (struct sockaddr*)&tldAddr, &len);
			int i = 0 , g = 0;
			char ips[1024];
			strcpy(ips,buffer);
			//bzero(buffer,MAXLINE);
			const char s[2] = ",";
			char *token = strtok(ips,s);
			while(token != NULL)
			{
				strcpy(lookup[k].ip[i++],token);
				token = strtok(NULL,s);
                       }
			lookup[k].nip = i;             //add to the local server table
			k++;
		}

		else
		{
                  	for (int i = 0; i < lookup[position].nip; i++)
                  	{
                  		strcat(buffer,lookup[position].ip[i]);
                  		strcat(buffer,",");
                  	}
		}

		addr_size = sizeof(client);
		sendto(sockfd, buffer,sizeof(buffer), 0, (struct sockaddr*)&client, addr_size);

		printf("\nIP Addresses sent to client...... : %s", buffer);
 	}

 	FD_CLR(sockfd, &readfds);


    }

    close(tldfd);
    close(sockfd);
    return 0;

}

/*OUTPUT:
~/ $ ./local 5004

IP Addresses sent to client...... : 216.58.193.68,
IP Addresses sent to client...... : 14.139.161.7,
IP Addresses sent to client...... : 98.137.11.163,98.137.11.164,
IP Addresses sent to client...... : 14.139.161.7,
IP Addresses sent to client...... : 172.217.14.195,
IP Addresses sent to client...... : 23.194.212.104,23.194.212.120,
*/