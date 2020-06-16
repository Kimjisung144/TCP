#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4
void error_handling(char *message);
int main(int argc, char *argv[])
{
	int sock;
	char opmsg[BUF_SIZE];
	int result, opnd_cnt, i;
	struct sockaddr_in serv_adr;

	if (argc!=3)
	{
		printf("usage:%s <port>\n,argv[0]");
		exit(1);
	}
	sock=socket(PF_INET,SOCK_STREAM,0);
	if(sock==-1)//소켓 생성 잘안될떄//
		error_handling("socket() error");
	memset(&serv_adr,0,sizeof(serv_adr));//서버주소 초기화//
	serv_adr.sin_family=AF_INET;//ip4 를 주소 체계설정//
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);//ip주소 할당 
	serv_adr.sin_port=htons(atoi(argv[2]));//port 번호 초기화

	if(connect(sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)//연결요청 동시에 서버에 클라이언트 주소 전송
		error_handling("connect() error");
	else
		puts("Connected......");

	fputs("operand couunt:",stdout);
	scanf("%d",&opnd_cnt);
	opmsg[0]=(char)opnd_cnt;
	for (i=0;i<opnd_cnt;i++)
	{
		printf("operand %d",i+1);
		scanf("%d",(int*)&opmsg[i*OPSZ+1]);
	}

	fgetc(stdin);
	fputs("operator:",stdout);
	scanf("%c",&opmsg[opnd_cnt*OPSZ+1]);
	write(sock,opmsg,opnd_cnt*OPSZ+2);
	read(sock,&result,RLT_SIZE);

	printf("operation result:%d\n",result);
	close (sock);
	return 0;
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}

