#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock;//소켓생성//
	struct sockaddr_in serv_addr;//소켓주소//
	char message[BUF_SIZE];//메세지 크기//
	int str_len;
	if(argc!=3)//ip sock port//
	{
		printf("usage:%s <IP> <port> \n",argv[0]);
		exit(1);
	}
	sock=socket(PF_INET,SOCK_STREAM,0);//
	if(sock==-1)//소켓 생성 잘안될떄//
		error_handling("socket() error");

	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;//ip버전 4 사용
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);//서버주소 사용
	serv_addr.sin_port=htons(atoi(argv[2]));//port 주소 사용

	if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)//연결요청 동시에 서버에 클라이언트 주소 전송
		error_handling("connect() error");
	else
		puts("connected..........");
	while(1)
	{
		fputs("input message(q to quit):",stdout);
		fgets(message,BUF_SIZE,stdin);

		if(!strcmp(message,"q\n")||!strcmp(message,"q\n"))
			break;

		write(sock,message,strlen(message));
		str_len=read(sock,message,sizeof(message)-1);
		message[str_len]=0;
		printf("message from server :%s \n",message);
	}
	close(sock);//소켓종료
	return 0;
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}