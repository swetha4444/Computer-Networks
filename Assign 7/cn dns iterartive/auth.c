//Only .COM auth
#include "table.h"

int main()
{
	struct sockaddr_in local,tld_addr;
	int sockfd,tld_fd,n;
	socklen_t addr_size, len;
	char buffer[MAXLINE], reply[50], *ip;
	struct hostent *he;
	
	sockfd = setConn(&local, authLocPORT, 1, "Local Server");
    tld_fd = setConn(&tld_addr,authTldPORT,1,"TLD Server");
	

    while(1)
    {	    
            bzero(&buffer,sizeof(buffer));
            
            /*
            Communicating with TLD
            */
            addr_size = sizeof(tld_addr);
            //recv hostname from  tld
            recvfrom(tld_fd, &buffer, sizeof(buffer), 0, (struct sockaddr*)&tld_addr, &addr_size);
            printf("\nReceived IP request from local server for %s",buffer);
            //getting IP
            he = gethostbyname(buffer);
            ip = inet_ntoa(*((struct in_addr *)he->h_addr_list[0]));//get the IP address from host entry
            //Sending IP to TLD
            strcpy(reply, ip);
            sendto(tld_fd,&reply, sizeof(reply), 0, (struct sockaddr*) &tld_addr, addr_size);

            /*
            Communicating with Local
            */
            len = sizeof(local);
            //Sending IP to Local
            sendto(sockfd,&reply, sizeof(reply), 0, (struct sockaddr*) &local, addr_size);
            printf("\nReplied back with IP %s to local server.\n", ip);
   }
   
   close(sockfd);
   return 0;
}           
	
