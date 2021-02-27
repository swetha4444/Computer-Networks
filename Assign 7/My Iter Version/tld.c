//Only .COM TLD
#include "table.h"
#define PORT "4008" //here we only use .com auth


int main()
{
    int sockfd;
    char buffer[MAXLINE];
    socklen_t addr_size, len;
    struct sockaddr_in local;
    
    sockfd = setConn(&local, tldPORT, 1, "local server");
    
    addr_size = sizeof(local);

    while(1)
    {	    
            bzero(&buffer,sizeof(buffer));
            
    	    //recv hostname from local
            recvfrom(sockfd, &buffer, sizeof(buffer), 0, (struct sockaddr*)&local, &addr_size);
            printf("\nReceived request from local server for %s",buffer);

	    strcpy(buffer,PORT);
	    
	    //send to local server
	    sendto(sockfd,&buffer, sizeof(buffer), 0, (struct sockaddr*) &local, addr_size);
	    printf("\nReplied back with port address [%s] of Authoritative Server.\n",buffer);
	    
	    
    }
    
    close(sockfd);
    return 0;
}
