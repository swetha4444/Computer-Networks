#include<netdb.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#define MAX 80
#define PORT 8201
#define SA struct sockaddr

void chat(int sockfd)
{
	char buff[MAX];
	int n = 0;
	printf("\n");
	while(1)
	{
		n = 0;
		bzero(buff,sizeof(buff));
		printf("Client: ");
		while((buff[n++] = getchar())!='\n');
		if(strncmp("exit",buff,4)== 0)
		{
			printf("\nCLIENT EXITS\n");
			write(sockfd,buff,sizeof(buff));
			break;
		}
		write(sockfd,buff,sizeof(buff));

	
		//read from server
		bzero(buff,sizeof(buff));
	
		read(sockfd,buff,sizeof(buff));
		if(strncmp("exit",buff,4)== 0)
		{
			printf("\nSERVER LEFT THE CHAT\n");
			break;
		}
		else
			printf("\nServer: %s\n",buff);	
	}
}

int main()
{
	int sockfd,server_fd;
	struct sockaddr_in servaddr,cli;
	char buff[MAX];
	int n = 0;
	
	//Socket creation and verification

	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1)
	{
		printf("Socket creation failed..!\n");
		exit(0);
	}
	
	bzero(&servaddr,sizeof(servaddr));

	//Assign IP,PORT

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	//Connection
	
	if(connect(sockfd,(SA*)&servaddr,sizeof(servaddr))!=0)
	{
		printf("Connection Failed..\n");
		exit(0);
	}


	//Chat function
	chat(sockfd);
	
	close(sockfd);
}	
		
