#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <signal.h>

#define BUF_SIZE 30
void error_handling(char *message);
void read_routine(int sock, char *buf);
void write_routine(int sock, char *buf);

int main(int argc, char *argv[])
{
	int sock;
	pid_t pid;
	char buf[BUF_SIZE];
	struct sockaddr_in serv_adr;
	if(argc!=3)//ip sock port//
	{
		printf("usage:%s <IP> <port> \n",argv[0]);
		exit(1);
	}
	sock = socket(PF_INET,SOCK_STREAM,0);

	memset(&serv_adr,0,sizeof(serv_adr));//서버주소 초기화//
	serv_adr.sin_family=AF_INET;//ip4 를 주소 체계설정//
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);//ip주소 할당 
	serv_adr.sin_port=htons(atoi(argv[2]));//port 번호 초기화

	if(connect(sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)//연결요청 동시에 서버에 클라이언트 주소 전송
		error_handling("connect() error");
	pid=fork();
	if(pid==0)
		write_routine(sock,buf);
	else
		read_routine(sock,buf);

	close(sock);
	return 0;
}
void read_routine(int sock,char *buf)
{
	while(1)
	{
		int str_len = read(sock,buf,BUF_SIZE);
		if(str_len==0)
			return;
		buf[str_len]=0;
		printf("Message from server:%s\n",buf);
	}
}
void write_routine(int sock, char *buf)
{
	printf("insert message(q to quit):");
	while(1)
	{
		fgets(buf,BUF_SIZE,stdin);
		if (!strcmp(buf,"q\n")||!strcmp(buf,"q\n"))
		{
			shutdown(sock,SHUT_WR);
			return;
		}
		write(sock,buf,strlen(buf));
	}
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}

