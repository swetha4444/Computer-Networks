#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<fcntl.h>

char message[1024], hammed_msg[1024];
int r,m;

void process();
int isPowOf2(int x);

int main(int argc,char **argv) {

	int len;
	int sockfd,n,newfd;
	struct sockaddr_in servaddr,cliaddr;

	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
		perror("cannot create socket");

	bzero(&servaddr,sizeof(servaddr));

	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=INADDR_ANY;
	servaddr.sin_port=htons(7221);

	connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));

	printf("\nWaiting for server message !!!... \n\n");

	recv(sockfd, hammed_msg, sizeof(hammed_msg), 0);

	printf("Message recieved from server : %s\n", hammed_msg);

	process();

	printf("\nMessage intended to be sent : %s\n\n", message);

	close(sockfd);
	close(newfd);
    return 0;
}

int isPowOf2(int x) {
	return ((x) & (x+1)) == 0;
}

void process() {
	int k=0, i=0,j, val;
	int len = strlen(hammed_msg);

	r = 0;
	val = 1;
	while(val < len+1) {
		val*=2;
		r++;
	}

	m = len - r;

	printf("\nNo of redundant bits used : %d\n", r);
	printf("No of message bits used   : %d\n", m);

	printf("\nCalculated redundant Bits : ");

	int error_bit = 0;
	for(j=r-1;j>=0;j--) {
		int val = 0;
		for(i=0;i<len;i++) {
			int temp = ((i+1) & (1<<j));
			if(temp) {
				val ^= (hammed_msg[i] - '0');
			}
		}
		error_bit = error_bit + (val * (1<<j));
		printf("%d", val);
	}
	printf("\n");

	if(error_bit) {
		printf("\nError found at Bit %d\n", error_bit);
		hammed_msg[error_bit-1] = (1-(hammed_msg[error_bit-1]-'0'))+'0';
		printf("Data after correction : %s\n", hammed_msg);
	}
	else {
		printf("\nNo error, message arrived was correct\n");
	}

	i=0;

	while(k < len) {
		if(!isPowOf2(k)) {
			message[i++] = hammed_msg[k];
		}
		k++;
	}

	message[i] = '\0';

}