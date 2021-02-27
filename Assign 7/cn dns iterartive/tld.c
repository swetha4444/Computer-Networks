//Only .COM TLD
#include "table.h"
#define PORT "4006" //here we only use .com auth


int main()
{
    int sockfd,auth_fd;
    char buffer[MAXLINE];
    socklen_t addr_size, len;
    struct sockaddr_in root,auth_addr;
    
    sockfd = setConn(&root, tldRootPORT, 1, "Root Server");
    auth_fd = setConn(&auth_addr,authTldPORT,0,"Authoritative Server")
    

    while(1)
    {	    
        bzero(&buffer,sizeof(buffer));

        /*
            Communicating with ROOT
        */
        addr_size = sizeof(root);
    	//recv hostname from root
        recvfrom(sockfd, &buffer, sizeof(buffer), 0, (struct sockaddr*)&root, &addr_size);
        printf("\nReceived request from local server for %s",buffer);
	    //send to root server
	    sendto(sockfd,&buffer, sizeof(PORT), 0, (struct sockaddr*) &local, addr_size);
	    printf("\nReplied back with port address [%s] of Authoritative Server.\n",buffer);

        /*
            Communicating with Auth 
        */
	    len = sizeof(auth_addr);
	    //send hostname to auth	 
	    sendto(auth_fd, &hostname, sizeof(buffer), 0, (struct sockaddr*) &auth_addr, len);
	    //recv IP addr from auth
	    recvfrom(auth_fd,&buffer, MAXLINE, 0, (struct sockaddr*)&auth_addr, &len);
	    printf("\nAuthoritative Server replied with IP address: %s.",buffer);
	    
    }
    
    close(sockfd);
    return 0;
}
