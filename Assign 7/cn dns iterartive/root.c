//Root will be like a server to local
#include "table.h"
#define PORT "4004"


int main()
{
    int sockfd,tld_fd;
    char buffer[MAXLINE];
    socklen_t addr_size, len;
    struct sockaddr_in local,tld_addr;
    
    sockfd = setConn(&local, rootLocPORT, 1, "Local server");
    tld_fd  = setConn(&tld_addr, tldRootPORT, 0, "TLD server");   //do not bind here
    

    while(1)
    {   
        /*
            Communicating with LOCAL 
        */
        addr_size = sizeof(local);
    	//recv hostname from local
    	bzero(buffer,MAXLINE);
        recvfrom(sockfd, &buffer, MAXLINE, 0, (struct sockaddr*)&local, &addr_size);
        printf("\nReceived request from local DNS server for %s",buffer);
	    printf("\nReplied back with port address [%s] of TLD \n",buffer);
	    //send to local server
	    sendto(sockfd, &buffer, sizeof(PORT), 0, (struct sockaddr*) &local, addr_size);

        /*
            Communicating with TLD
        */
        len = sizeof(tld_addr);
        //send hostname to tld
        bzero(buffer, MAXLINE);	 
        sendto(tld_fd, &hostname, sizeof(buffer), 0, (struct sockaddr*) &tld_addr, len);
        //recv authaddr from tld
        recvfrom(tld_fd, buffer, MAXLINE, 0, (struct sockaddr*)&tld_addr, &len);
        printf("\nTLD replied with address port [%s] to request the Authoritative Server.\n\tRequesting Authoritative Server.............",buffer);
        
    }
    
    close(sockfd);
    return 0;
}
    

    