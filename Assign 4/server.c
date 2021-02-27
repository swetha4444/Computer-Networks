#include<netdb.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#define MAX 80
#define PORT 8201
#define SA struct sockaddr

int clientCount = 1;

void error(char *msg)
{
	perror(msg);
	exit(1);
}

void chat(int sockfd,int connfd,int clientIndex)

{
	char buff[MAX];
	int n;
	while(1)
	{
		//Clear buffer
		bzero(buff,sizeof(buff));
	
		//Read message from client
		read(connfd,buff,sizeof(buff));
		if(strncmp("exit",buff,4)== 0)
		{
			printf("\nCLIENT %d LEFT THE CHAT\n",clientIndex);
			break;
		}
		else
			printf("\nClient %d: %s\n",clientIndex,buff);
		
		//Write message to client
		n = 0;
		bzero(buff,MAX);
		printf("Server: ");
		while((buff[n++] = getchar())!='\n');

		if(strncmp("exit",buff,4)== 0)
		{
			printf("\nSERVER EXITS\n");
			close(sockfd);
		}
		write(connfd,buff,sizeof(buff));
		
	}

}


int main()
{
	int sockfd,connfd,pid;
	struct sockaddr_in servaddr,cli;
	char buff[MAX];
	int n = 0 , len;
	
	//Socket creation and verification

	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1)
		error("Socket creation failed.\n");

	//Reset Server Address
	bzero(&servaddr,sizeof(servaddr));

	//Assign IP,PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);
	
	//Binding to port
	if((bind(sockfd,(SA*)&servaddr,sizeof(servaddr)))!=0)
		error("Address Failed Binding.\n");

	//Listen for client
	if(listen(sockfd,5)!=0)
		error("Listening Failed.\n");
	
	while(1)
	{
		//Accept client
		len = sizeof(cli);
		connfd = accept(sockfd,(SA*)&cli,&len);
		if(connfd<0)
			error("Not Accepted.\n");
		else
			printf("\nCLIENT %d ENTERED THE CHAT\n",clientCount);
			
		pid = fork();
		if (pid == -1) //error
		{
			close(connfd);
			error("Error in creating process\n");
			continue;
		}
		else if(pid > 0)
		{
			close(connfd);
			clientCount++;
			continue;
		}
		else if(pid ==0){
			//Chat function
			chat(sockfd,connfd,clientCount);
			//close(connfd);
			bzero(buff,MAX);
			break;
		}
		
		close(sockfd);
	}
	
}	
