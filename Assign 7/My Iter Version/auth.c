//Only .COM auth
#include "table.h"

int main()
{
	struct sockaddr_in local;
	int sockfd, n;
	socklen_t addr_size, len;
	char buffer[MAXLINE], reply[50], *ip;
	struct hostent *he;
	
	sockfd = setConn(&local, authPORT, 1, "local server");
	
	addr_size = sizeof(local);

    while(1)
    {	    
            bzero(&buffer,sizeof(buffer));
            
            //recv hostname from local
            recvfrom(sockfd, &buffer, sizeof(buffer), 0, (struct sockaddr*)&local, &addr_size);
            printf("\nReceived IP request from local server for %s",buffer);
            
            he = gethostbyname(buffer);
            ip = inet_ntoa(*((struct in_addr *)he->h_addr_list[0]));//get the IP address from host entry
            
            //sending IP to local
            strcpy(reply, ip);
            sendto(sockfd,&reply, sizeof(reply), 0, (struct sockaddr*) &local, addr_size);
            printf("\nReplied back with IP %s to local server.\n", ip);
   }
   
   close(sockfd);
   return 0;
}           
	
