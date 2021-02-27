#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
int main(int argc,char **argv)
{
    int len;
    int sockfd,newfd,n;
    struct sockaddr_in servaddr,cliaddr;
    char buff[1024];
    char str[1000];
    char buff2[1024];
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
        perror("cannot create socket");
    bzero(&servaddr,sizeof(servaddr));
    
    servaddr.sin_family=AF_INET;   //uses the internet address family
    servaddr.sin_addr.s_addr=INADDR_ANY; //INADDR_ANY used when you don't need to bind to a specific IP
    servaddr.sin_port=htons(7228); //connects sd to port 7728

    if(bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
        perror("Bind error");
    listen(sockfd,2);
    len=sizeof(cliaddr);
    newfd=accept(sockfd,(struct sockaddr*)&cliaddr,&len);

    n=read(newfd,buff,sizeof(buff));
    printf("Message from Client: %s",buff);
    int i;
    n=write(newfd,buff,sizeof(buff));
    printf("\n\nMessage sent: %s",buff);
    close(sockfd);
    close(newfd);
    return 0;
}

/*

Output:

vignesh@vignesh:~/Desktop/A2$ ./s
Message from Client: Hi Server
This is BV from server.

Message sent: Hi Server
This is BV from server.

*/
