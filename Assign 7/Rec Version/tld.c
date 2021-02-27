//TLD SERVER   -- Considering a situation with ultiple local servers trying to establish connection  with one TLD server

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
Table lookUPCOM[10]; //table of .com domains
Table lookUPIN[10];  //table of .in domains
Table lookUPEDU[10];  //table of .edu domains

int exten(char url[30])      //to figure which extension
{
	const char s[2] = ".";
	char *token = strtok(url,s);
	char extn[20] ;
	while(token != NULL)
	{
        	strcpy(extn,token);
        	token = strtok(NULL,s);
        }

        if(strcmp(extn , "com") == 0)
        	return 0;
        else if(strcmp(extn, "in") == 0)
        	return 1;
        else
        	return 2;
}


int main(int argc, char **argv)
{
    int port = 55621;                 //Port the local server would have to connect to
    int sockfd, listenfd, udpfd, ready, position;
    char buffer[1024];
    fd_set readfds;
    struct hostent *host;       //a structure for entry of host's database
    socklen_t addr_size;
    int c = 0, in = 0, e = 0;
    struct sockaddr_in serverAddr, client, sock_addr;

    struct timeval tv;
    tv.tv_sec  = 1;
    tv.tv_usec = 0;

    sockfd = socket(AF_INET,SOCK_DGRAM,0);

    if(sockfd < 0)
    {
        printf("\nError in establishing Socket Connection.........!!!");
        exit(1);
    }

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(port);
    serverAddr.sin_family = AF_INET;

    if(bind(sockfd, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) != 0)
    {
         printf("\nSocket Bind Failed");
         exit(1);
    }

    addr_size = sizeof(client);

    while(1)
    {
    	FD_SET(sockfd, &readfds);

    	int activity = select(sockfd + 1, &readfds, NULL, NULL, &tv);

    	if (activity < 0)
    	{
             printf("Error in select()");
             exit(EXIT_FAILURE);
	}

	if(FD_ISSET(sockfd, &readfds))
	{
        	addr_size = sizeof(client);
        	recvfrom(sockfd, buffer, MAXLINE, 0, (struct sockaddr*)&client, &addr_size);
        	printf("\nURL received : %s" , buffer);
               char hostname[1024];
               strcpy(hostname, buffer);
               int check = exten(buffer);
               bzero(buffer, MAXLINE);
               strcpy(buffer, "");
               if (check == 0)                 //if the received url is of .com domain figure out the position along the table
               {
        		position = checkTable(lookUPCOM,hostname, c);

        		if (position == -1)                                 //authoritative server is called
        		{
        	        int g;
        			host = gethostbyname(hostname);
        			strcpy(lookUPCOM[c].domainName,hostname);
    				for (int i = 0; host->h_addr_list[i]; ++i)
				    {
					    sock_addr.sin_addr = *((struct in_addr*) host->h_addr_list[i]);                     //finding the list of ip's for the authoritative server
					    inet_ntop(AF_INET, &sock_addr.sin_addr, lookUPCOM[c].ip[i], sizeof(lookUPCOM[c].ip[i]));

					    strcat(buffer,lookUPCOM[c].ip[i]);            //adding the ip's to the buffer
					    strcat(buffer,",");
					    g = i;

                       	}
                       	lookUPCOM[c].nip = g + 1;    // adding the new domain to the .com table
                       	//bzero(buffer, MAXLINE);

        			c++;
               	}

               	else
               	{
                  		for (int i = 0; i < lookUPCOM[position].nip; i++)
                  		{
                  			strcat(buffer,lookUPCOM[position].ip[i]);               //add all the ip's to the buffer
                  			strcat(buffer,",");
                  		}
                  	}
               }


               else if(check == 1)         //if the received url is of .com domain figure out the position along the table
               {
        		position = checkTable(lookUPIN,hostname, in);

        		if (position == -1)                                 //authoritative server is called
        		{
        	        	int g;
        			host = gethostbyname(hostname);
        			strcpy(lookUPIN[in].domainName,hostname);
    				for (int i = 0; host->h_addr_list[i]; ++i)            //finding the list of ip's for the authoritative server
				    {
					    sock_addr.sin_addr = *((struct in_addr*) host->h_addr_list[i]);
					    inet_ntop(AF_INET, &sock_addr.sin_addr, lookUPIN[in].ip[i], sizeof(lookUPIN[in].ip[i]));

					    strcat(buffer,lookUPIN[in].ip[i]);
					    strcat(buffer,",");
					    g = i;

                    }
                       	lookUPIN[in].nip = g + 1;        // adding the new domain to the .in table
                       	//bzero(buffer, MAXLINE);

        			in++;
               	}

               	else
               	{
                  		for (int i = 0; i < lookUPIN[position].nip; i++)
                  		{
                  			strcat(buffer,lookUPIN[position].ip[i]);
                  			strcat(buffer,",");
                  		}
                  	}
                 }


               else
               {
        		position = checkTable(lookUPEDU,hostname, e);

        		if (position == -1)                                 //authoritative server is called
        		{
        	        	int g;
        			host = gethostbyname(hostname);
        			strcpy(lookUPEDU[e].domainName,hostname);
    				for (int i = 0; host->h_addr_list[i]; ++i)
				{
					sock_addr.sin_addr = *((struct in_addr*) host->h_addr_list[i]);             //finding the list of ip's for the authoritative server
					inet_ntop(AF_INET, &sock_addr.sin_addr, lookUPEDU[e].ip[i], sizeof(lookUPEDU[e].ip[i]));

					strcat(buffer,lookUPEDU[e].ip[i]);
					strcat(buffer,",");
					g = i;

                       	}
                       	lookUPEDU[e].nip = g + 1;
                       	//bzero(buffer, MAXLINE);

        			e++;
               	}

               	else
               	{
                  		for (int i = 0; i < lookUPEDU[position].nip; i++)
                  		{
                  			strcat(buffer,lookUPEDU[position].ip[i]);
                  			strcat(buffer,",");
                  		}
                  	}
                 }

               addr_size = sizeof(client);
               sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client, addr_size);
               printf("\nIP Addresses sent to client... : %s",buffer);

         }

         FD_CLR(sockfd, &readfds);
  }

 close(sockfd);
 return 0;
}