#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sd;
	FILE *fp;

	char buf[BUF_SIZE];
	int read_cnt;

	struct sockaddr_in serv_adr;
	if(argc!=3)//ip sock port//
	{
		printf("usage:%s <IP> <port> \n",argv[0]);
		exit(1);
	}
	fp = fopen("receive.dat","wb");//파일을 보내겠다고 보냄
	sd = socket(PF_INET,SOCK_STREAM,0);

	memset(&serv_adr,0,sizeof(serv_adr));//서버주소 초기화//
	serv_adr.sin_family=AF_INET;//ip4 를 주소 체계설정//
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);//ip주소 할당 
	serv_adr.sin_port=htons(atoi(argv[2]));//port 번호 초기화

	connect(sd,(struct sockaddr*)&serv_adr,sizeof(serv_adr));

	while((read_cnt=read(sd,buf,BUF_SIZE))!=0)
		fwrite((void*)buf,1,read_cnt,fp);
		puts("Received file data");
		write(sd,"Thank you",10);
		fclose(fp);
		close(sd);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
} 