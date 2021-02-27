#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<fcntl.h>

char message[1024], hammed_msg[1024];
int r,m;

void input();
void process();
int isPowOf2(int x);

int main(int argc,char **argv) {

	int len;
	int sockfd,newfd,n;
	struct sockaddr_in servaddr,cliaddr;


	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
		perror("cannot create socket");

	bzero(&servaddr,sizeof(servaddr));

	servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=INADDR_ANY;
	servaddr.sin_port=htons(7221);

	if(bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0) {
		perror("Bind error");
        return 0;
    }


	listen(sockfd,2);

	len=sizeof(cliaddr);
	newfd=accept(sockfd,(struct sockaddr*)&cliaddr,&len);

	input();
	process();
	printf("\nMessage transferred to the Client : %s\n\n", hammed_msg);
    // newfd
	send(newfd,hammed_msg,m+r,0);

	close(sockfd);
	close(newfd);
	return 0;
}

void input() {
	printf("\nMessage to be transmitted : ");
	scanf("%s", message);
	m = strlen(message);
	printf("\nNo of message bits   : %d", m);

	r = 0;
	int val = 1; // val = 2^r
	while(val < m+r+1) {
		val*=2;
		r++;
	}
	printf("\nNo of redundant bits  : %d\n", r);
}


int isPowOf2(int x) {
	return ((x) & (x+1)) == 0;
}

void process() {
	int k=0, len=0, i,j, val;
	while(k < m) {
		if(isPowOf2(len)) {
			hammed_msg[len] = '0';
		} else {
			hammed_msg[len] = message[k++];
		}
		len++;
	}

	hammed_msg[len] = '\0';

	for(i=0;i<len;i++) {
		for(j=0;j<r;j++) {
			int temp = ((i+1) & (1<<j));
			if(temp) {
				int indx = (1<<j) - 1;
				hammed_msg[indx] = ((hammed_msg[indx]-'0') ^ (hammed_msg[i]-'0')) + '0';

			}
		}
	}

	printf("\nHammed message formed : %s\n", hammed_msg);

	val = 0;
	while(isPowOf2(val)) {
		val = rand() % len;
	}

	printf("\nError induced in bit (0 means no error) : %d", val+1);

	hammed_msg[val] = (1-(hammed_msg[val]-'0'))+'0';

}