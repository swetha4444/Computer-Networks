#include "table.h"

Table lookup[30];
int cli_fd,root_fd,auth_fd;
struct sockaddr_in client_addr, local_addr, root_addr,auth_addr;

int main()
{
    int position, k = 0;
    char buffer[MAXLINE],hostname[1024],IPs[1024];
    socklen_t addr_size, len;

    
    cli_fd  = setConn(&local_addr, localCliPORT, 1, "Client");  //bind here since server
    root_fd = setConn(&root_addr, rootLocPORT, 0, "Root Server");   //do not bind here
    auth_fd = setConn(&auth_addr, authLocPORT, 0, "Authoritative Server"); //do not bind here

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
            printf("\nCommunicating with the ROOT Server");
            //send hostname to root
            bzero(buffer, MAXLINE);
            sendto(root_fd, &hostname, sizeof(hostname), 0, (struct sockaddr*)&root_addr, len);
            //recv tld addr from root
            recvfrom(root_fd, &buffer, MAXLINE, 0, (struct sockaddr*)&root_addr, &len);
            printf("\nRoot replied with address port [%s] to request the TLD Server.\n\tRequesting TLD Server.............",buffer);
            
           /*
                Communicating with Auth 
           */
           len = sizeof(auth_addr);
           //recv IP addr from auth
           recvfrom(auth_fd,&buffer, MAXLINE, 0, (struct sockaddr*)&auth_addr, &len);
           printf("\nAuthoritative Server replied with IP address: %s.",buffer);

           //Saving IP in the lookUp table
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