#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>

int main(int argc,char **argv){
    int len;
    int sockfd,n;
    struct sockaddr_in servaddr,cliaddr;
    char str[1000];
    char buff1[1024];
    char buff2[1024];
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
        perror("cannot create socket");        
    bzero(&servaddr,sizeof(servaddr));//erases the data in sizeof(servaddr) bytes starting from servaddr

    servaddr.sin_family=AF_INET; //uses the internet address family
    servaddr.sin_addr.s_addr=inet_addr(argv[1]);    //connects to the IP address passed as argument
    servaddr.sin_port=htons(7228);  //connects sd to port 7728
    
    connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    //Sending Message
    int i=0;
    printf("Enter the message till full stop(.)\n");
    printf("Client: ");
    while(1)
    {
        buff1[i] = getchar();
        if (buff1[i]=='.')
            break;
        i++;
    }
    n=write(sockfd,buff1,sizeof(buff1));
    n=read(sockfd,buff2,sizeof(buff2));
    printf("\nMessage from server: %s",buff2);
    close(sockfd);
    return 0;
}


/*

Output:

vignesh@vignesh:~/Desktop/A2$ ./c 127.0.0.1
Enter the message till full stop(.)
Client: Hi Server
This is BV from server.

Message from server: Hi Server
This is BV from server.

*/
