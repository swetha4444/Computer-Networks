#include "table.h"

Table lookup[30];
int cli_fd, root_fd, tld_fd, auth_fd;
struct sockaddr_in client_addr, local_addr, root_addr, tld_addr, auth_addr;

int main()
{
    int position, k = 0;
    char buffer[MAXLINE],hostname[1024],IPs[1024];
    socklen_t addr_size, len;

    
    cli_fd  = setConn(&local_addr, cliPORT, 1, "client");  //bind here since server
    root_fd = setConn(&root_addr, rootPORT, 0, "root server");   //do not bind here
    tld_fd  = setConn(&tld_addr, tldPORT, 0, "TLD server");   //do not bind here
    auth_fd = setConn(&auth_addr, authPORT, 0, "authoritative server"); //do not bind here

    addr_size = sizeof(client_addr);
    
    while(1)
    {
    	bzero(&buffer, sizeof(buffer));
    	//recv hostname from clients
	recvfrom(cli_fd, &buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr,&addr_size);
	printf("\n\nReceived a request for IP Address of %s from a client.",buffer);
	
	strcpy(hostname, buffer);
	position = checkTable(lookup, buffer, k);
	
	
	if (position == -1)   //if not in lookup
	{ 
	  printf("\nLocal table does not have an entry for %s.\n\tRequesting Root Server.............",hostname);
	  
	  //store domain name in lookUp table
	  strcpy(lookup[k].domainName,hostname);


	  /*
	  	Communicating with ROOT 
	  */
	  len = sizeof(root_addr);
	  //send hostname to root
	  bzero(buffer, MAXLINE);
	  sendto(root_fd, &hostname, sizeof(hostname), 0, (struct sockaddr*)&root_addr, len);
	  //recv tld addr from root
	  recvfrom(root_fd, &buffer, MAXLINE, 0, (struct sockaddr*)&root_addr, &len);
	  printf("\nRoot replied with address port [%s] to request the TLD Server.\n\tRequesting TLD Server.............",buffer);
	  
	  
	  /*
	  	Communicating with TLD
	  */
   	  len = sizeof(tld_addr);
	  //send hostname to tld
	  bzero(buffer, MAXLINE);	 
	  sendto(tld_fd, &hostname, sizeof(hostname), 0, (struct sockaddr*) &tld_addr, len);
	  //recv authaddr from tld
	  recvfrom(tld_fd, buffer, MAXLINE, 0, (struct sockaddr*)&tld_addr, &len);
	  printf("\nTLD replied with address port [%s] to request the Authoritative Server.\n\tRequesting Authoritative Server.............",buffer);
	  
	  
	  /*
	  	Communicating with Auth
	  */
	  len = sizeof(auth_addr);
	  //send hostname to auth
	  bzero(&buffer, MAXLINE);	 
	  sendto(auth_fd, &hostname, sizeof(hostname), 0, (struct sockaddr*) &auth_addr, len);
	  //recv authaddr from tld
	  recvfrom(auth_fd,&buffer, MAXLINE, 0, (struct sockaddr*)&auth_addr, &len);
	  printf("\nAuthoritative Server replied with IP address: %s.",buffer);
	    
	  
	  //Setting IP's in table
	  strcpy(lookup[k].ip,buffer);
	  k++;	  
   
       }
       
       else //IP in lookup
       {
          printf("\nLocal table has an entry for %s.\n",hostname);
          bzero(&buffer, MAXLINE);
          strcpy(buffer,lookup[position].ip);
          printf("\nLocal Server replied with IP address: %s.",buffer);
       }
       
      //send IPs to client
      addr_size = sizeof(client_addr);
      sendto(cli_fd,&buffer,sizeof(buffer), 0, (struct sockaddr*)&client_addr, addr_size);
   }
    
   close(cli_fd);
   return 0;

}