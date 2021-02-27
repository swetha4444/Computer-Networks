#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

#define SA struct sockaddr
#define SERVER_PORT 8000
#define MAX 256

int server_fd,n = 1;
struct sockaddr_in servaddr, clientaddr;


int error(char *msg)
{
	perror(msg);
	exit(1);
}


void ansOption(time_t cur_time, char option)
{
        struct tm *temp;
	time_t toronto_time;
	char time_buffer[MAX];
	int addrlen,year;

	temp = localtime(&cur_time);
	printf("\nRequest from Client %d: Option %c\n",n,option);

	if(option == '1')
		strftime(time_buffer, sizeof(time_buffer), "%x", temp);

	else if(option == '2')
		strftime(time_buffer, sizeof(time_buffer), "%A", temp);


	else if(option == '3')
		strftime(time_buffer, sizeof(time_buffer), "%B", temp);


	else if(option == '4')
	{
		year = temp->tm_year + 1900;
		sprintf(time_buffer, "%d", year);
	}

	else if(option == '5')
		strftime(time_buffer, sizeof(time_buffer), "%I:%M%p", temp);


        else if(option == '6')
	{	bzero(&time_buffer, sizeof(time_buffer));
		temp = gmtime(&cur_time); //GMT
		temp->tm_hour -= 5; //Toronto is -5H ahead of GMT
		toronto_time = mktime(temp); 				         
		temp = localtime(&toronto_time);
		strftime(time_buffer, sizeof(time_buffer), "%c", temp);
	}

        else
	{	printf("\n\tInvalid Option.\n");
		strcpy(time_buffer,"Invalid Option");
	}
	
	addrlen = sizeof(clientaddr);
	sendto(server_fd, time_buffer, sizeof(time_buffer), 0, (SA*)&clientaddr,addrlen);
	printf("\nMessage sent to Client %d\n",n);
}



int main()
{	
	int bytecount,addrlen, opt;
	char buf[MAX];
    time_t cur_time;
	
	//CREATE SOCKET
	if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) <= 0) 
        	error("\033[0;31mSOCKET ERROR\033[0m");

	//CLEAR servaddr
	bzero(&servaddr , sizeof(servaddr));

        // SET IP,PORT,FAMILY OF SERVER
	servaddr.sin_port = htons(SERVER_PORT);
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;

	//BIND 
	if((bind(server_fd, (SA*)&servaddr, sizeof(servaddr))) !=0 ) 
        	error("\033[0;31mBIND ERROR\033[0m");


	//RECIEVE DATA
	addrlen = sizeof(clientaddr);

	while(1)
	{
	    	bzero(buf,sizeof(buf));
		bytecount = recvfrom(server_fd, buf, sizeof(buf), 0,(SA*)&clientaddr, &addrlen);
		cur_time = time(NULL);
		ansOption(cur_time,buf[0]);
		n++;
	}
	
	return 0;
}



