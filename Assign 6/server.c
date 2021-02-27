#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SA struct sockaddr 
#define SERVER_PORT 4000
#define MAX 256
#define N_CLIENTS 3


struct arpCatche  
{  
    char IP[20];
    char MAC[20];    
};
struct arpCatche arpTable[10]; 

struct packet
{
    char srcIP[20];
    char srcMAC[20];
    char dstIP[20];
    char dstMAC[20];
    char data[20];
};


int error(char *msg)
{
	perror(msg);
	exit(1);
}


int lookUp(char IP[20],char MAC[20])
{
	if(strlen(MAC) == 0) //check for dest client_fd
		for (int i = 0; i < N_CLIENTS; i++)
			if(strcmp(IP,arpTable[i].IP) == 0)
				return i;
				
	else //check MAC and IP validity
		for (int i = 0; i < N_CLIENTS; i++)
			if(strcmp(IP,arpTable[i].IP) == 0)
				if(strcmp(MAC,arpTable[i].MAC) == 0)
					return i;
	return -1;
}


int main(int argc, char const *argv[])
{
	int server_fd, client_fd,sock_fd;
	int addrlen,index;
	int clients[10];
    char buf[MAX], packet_str[MAX];
    struct packet pkt;
	struct sockaddr_in servaddr, clientaddr;  


	//IP & MAC of Server
	char serIP[]="192.168.1.1";
    	strcpy(pkt.srcIP,serIP);
	char serMAC[]="AF-45-E5-00-97-12";
    	strcpy(pkt.srcMAC,serMAC);

	//CREATE SOCKET
	if ((server_fd = socket(AF_INET,SOCK_STREAM,0)) <= 0) 
        	error("SOCKET FAILED");

	//RESET servaddr
	bzero(&servaddr, sizeof(servaddr)); 

    	//ASSIGN IP, PORT, FAMILY 
	servaddr.sin_port = htons(SERVER_PORT); 
	servaddr.sin_family = AF_INET; 

	//BIND TO PORT
	if ((bind(server_fd, (SA*)&servaddr, sizeof(servaddr))) != 0) 
        	error("BIND ERROR");

	//LISTEN ON PORT
	if ((listen(server_fd, 3)) != 0) 
        	error("LISTEN ERROR");
	printf("SERVER LISTENING ON PORT 4000\n");

	//ACCEPT CONNECTION FROM ALL CLIENTS
    index = 0;
	while(index < N_CLIENTS)
    {
		addrlen = sizeof(clientaddr);
		if ((client_fd = accept(server_fd, (SA*)&clientaddr, &addrlen)) <= 0) 
            	error("ACCEPT ERROR"); 
        printf("\nCLIENT %d CONNECTED", index+1);
		clients[index] = client_fd;

		bzero(buf, MAX);
		read(client_fd, buf, MAX); //IP Address of Client
		strcpy(arpTable[index].IP,buf);

		bzero(buf, MAX);
		read(client_fd, buf, MAX); //MAC Address of Client
		strcpy(arpTable[index].MAC,buf);   

		index ++;
	}

    //SERVER AS A HOST
     	
 	//GETTING DATA
	printf("\n\nEntering the details of the packet recieved:");

	printf("\n\tDestination IP\t: ");
	scanf("%s",pkt.dstIP);

	//IP Address of src
	printf("\tSource IP\t: %s",serIP);

	//MAC Address of src
	printf("\n\tSource MAC\t: %s",serMAC);

	//Data to be sent
	printf("\n\t16 bit data\t: ");
	scanf("%s",pkt.data);

	//ARP Request packet
	bzero(buf, MAX);
	strcat(buf,pkt.srcIP);
	strcat(buf," | ");
	strcat(buf,pkt.srcMAC);
	strcat(buf," | ");
	strcat(buf,pkt.dstIP);
	strcpy(packet_str,buf);

	//BROADCAST TO ALL CLIENTS
	printf("\nDeveloping ARP Request Packet:\n\t %s",buf);
	index = 0;
	while(index < N_CLIENTS)
	{
		write(clients[index], buf, sizeof(buf)); //packet
	    write(clients[index], pkt.dstIP, sizeof(pkt.dstIP)); //dstIP
	    index++;
	}            
	printf("\nARP Request Packet broadcasted.");
		
	//ARP:  Getting Reply
	printf("\n\nWaiting for ARP Reply....");		
	index = lookUp(pkt.dstIP,"");
		
	if( index == -1 )
	{
		printf("\nINVALID IP");
		bzero(buf, MAX);
	    strcpy(buf,"\nINVALID IP");
	    write(client_fd, buf, sizeof(buf));
	}
		
	else
	{
		//ARP:  Sending data to dst
		bzero(buf, MAX);
	    read(clients[index], buf, sizeof(buf)); //read dstMAC
	    strcpy(pkt.dstMAC,buf);		
	    strcat(packet_str," | ");
	    strcat(packet_str,pkt.dstMAC);
		    
	    if (lookUp(pkt.dstIP,pkt.dstMAC) == -1) //Checking validity
		    	printf("INVALID IP AND MAC ADDRESS MATCH");
		    
		printf("\nARP reply recieved\t: %s\n",packet_str);
		printf("\n\nSending packet to\t: %s",pkt.dstMAC);
	    strcat(packet_str," | ");
	    strcat(packet_str,pkt.data);
	    strcpy(buf,packet_str);
        write(clients[index], buf, sizeof(buf));
	    printf("\nPacket sent\t: %s\n",buf);				
	}    		           

	//CLOSE SOCKET 
	close(server_fd);
	   
	return 0;
}
