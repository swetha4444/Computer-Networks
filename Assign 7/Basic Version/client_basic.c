#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 7228

int main(int argc, char **argv){
	struct sockaddr_in server, client;
	int sockfd, n, addrlen, flag, choice = 1;
	char req_server[100], req_ip[50];

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	if(sockfd < 0){
		perror("Error in creating socket.\n");
	}

	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	addrlen = sizeof(server);

	while(choice){
		printf("\nEnter the Server's Name :\t");
		scanf("%s", req_server);
		sendto(sockfd, &req_server, sizeof(req_server), 0, (struct sockaddr*)&server, sizeof(server));

		recvfrom(sockfd, &req_ip, sizeof(req_ip), 0, (struct sockaddr*)&server, &addrlen);
		printf("The IP Address is :\t\t%s\n", req_ip);

		printf("\nContinue? (1 - YES, 0 - NO) -> ");
		scanf("%d", &choice);
	}

	close(sockfd);

	return 0;
}