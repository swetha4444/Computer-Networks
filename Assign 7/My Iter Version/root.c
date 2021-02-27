//Root will be like a server to local
#include "table.h"
#define PORT "4004"


int main()
{
    int sockfd;
    char buffer[MAXLINE];
    socklen_t addr_size, len;
    struct sockaddr_in local;
    
    sockfd = setConn(&local, rootPORT, 1, "local server");
    
    addr_size = sizeof(local);

    while(1)
    {   
    	//recv hostname from local
    	bzero(buffer,MAXLINE);
        recvfrom(sockfd, &buffer, MAXLINE, 0, (struct sockaddr*)&local, &addr_size);
        printf("\nReceived request from local DNS server for %s",buffer);
            
	    strcpy(buffer,PORT);
	    printf("\nReplied back with port address [%s] of TLD \n",buffer);
	    
	    //send to local server
	    sendto(sockfd, &buffer, sizeof(buffer), 0, (struct sockaddr*) &local, addr_size);
    }
    
    close(sockfd);
    return 0;
}
    
