#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#define SA struct sockaddr 
#define SERVER_PORT 4000
#define MAX 256

int error(char *msg){
	perror(msg);
	exit(1);
}

int main(int argc, char const *argv[]) {
	int bc,flag;
	char fileBuf[MAX],buf[MAX];
	int server_fd, client_fd, fd;
	struct sockaddr_in servaddr, clientaddr;	

	//CREATE SOCKET
	if ((server_fd = socket(AF_INET,SOCK_STREAM,0)) <= 0) 
		error("SOCKET FAILED");

	//RESET servaddr / ASSIGN IP, PORT, FAMILY 
	bzero(&servaddr, sizeof(servaddr)); 
	if ((inet_aton("127.0.0.1", &servaddr.sin_addr)) == 0) 
		error("IP ERROR");
	servaddr.sin_port = htons(SERVER_PORT); 
	servaddr.sin_family = AF_INET; 

    	//BIND TO PORT
	if ((bind(server_fd, (SA*)&servaddr, sizeof(servaddr))) != 0) 
		error("BIND ERROR");

    	//LISTEN ON PORT
	if ((listen(server_fd, 5)) != 0) 
		error("LISTEN ERROR");

    	//ACCEPT CONNECTION
	int len = sizeof(clientaddr);
	if ((client_fd = accept(server_fd, (SA*)&clientaddr, &len)) <= 0) 
		error("ACCEPT ERROR"); 

	//GET FILE NAME
	flag = read(client_fd, buf, MAX);
	printf("FILE REQUESTED TO BE READ: %s\n",buf );

	//OPEN FILE
	if((fd = open(buf,O_RDONLY)) <= 0) 
		error("FILE OPEN ERROR");

	//READ FILE
	if (fd < 0)
		error("FILE NOT FOUND");

	len = read(fd, fileBuf, MAX);
	close(fd);
	flag = write(client_fd, fileBuf, MAX);
	printf("\nFILE CONTENTS TRANSFERRED\n");

	//CLOSE
	bzero(buf, MAX); 
	bzero(fileBuf, MAX); 
	close(server_fd);	
	close(client_fd);
	return 0;
}
