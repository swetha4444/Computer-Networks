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
	char buf[MAX],file_name[20];
	int flag;
	int sock_fd = 0, server_fd;
	struct sockaddr_in servaddr; 

	//CREATE SOCKET
	if ((sock_fd = socket(PF_INET,SOCK_STREAM,0)) <= 0) 
		error("SOCKET FAILED");

	//RESET servaddr / ASSIGN IP, PORT, FAMILY  OF **SERVER**
	bzero(&servaddr, sizeof(servaddr)); 
	if ((inet_aton("127.0.0.1", &servaddr.sin_addr)) == 0) 
		error("IP ERROR");
	servaddr.sin_port = htons(SERVER_PORT); 
	servaddr.sin_family = AF_INET; 

    	//CONNECT
	if ((connect(sock_fd, (SA*)&servaddr, sizeof(servaddr))) != 0) 
		error("CONNECT ERROR");

	//SEND FILE NAME
	printf("\nEnter the File Location: ");
	scanf(" %[^\n]", file_name);
	flag = write(sock_fd, file_name, MAX);

	//GET CONTENT BACK
	flag = read(sock_fd, buf,  MAX);
	printf("FILE:\n%s\n", buf);
	int newfd = open("newfile.txt", O_WRONLY | O_CREAT);
	printf("FILE SAVED AS: newfile.txt\n");
	write(newfd, buf, MAX);

	//CLOSE
	bzero(buf, MAX); 
	close(sock_fd);
	return 0;
}
