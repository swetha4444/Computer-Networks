#include "table.h"

int main()
{
	char buffer[MAXLINE],IP[MAXLINE];
	char hostname[200] ;
	int sockfd;
	struct sockaddr_in serveraddr;
	socklen_t addr_size;

	//Conn with Local Server
	bzero(&serveraddr, sizeof(serveraddr));
	sockfd = setConn(&serveraddr, localCliPORT, 0, "Local server");
	
	addr_size = sizeof(serveraddr);
	bzero(&buffer, sizeof(buffer));

	while(1)
	{
		printf("\nEnter the host name: ");
    	scanf("%s", hostname);
    	strcpy(buffer,hostname);
    		
    	//send hostname to local
    	sendto(sockfd, &buffer, sizeof(buffer), 0, (struct sockaddr*)&serveraddr, addr_size);
		
    	//recv IP from local
    	recvfrom(sockfd, &buffer, sizeof(buffer), 0, (struct sockaddr*)&serveraddr, &addr_size);
    	printf("The IP Address is :\t\t%s\n", buffer);
    		
	}

	close(sockfd);
	return 0;
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
