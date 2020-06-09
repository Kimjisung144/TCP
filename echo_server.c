#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc, char *argv[])//argc 
{
	int serv_sock;//서버 소켓//
	int clnt_sock;//클라이언트 소켓//
	char message[BUF_SIZE];
	int str_len,i;

	struct sockaddr_in serv_adr;//서버 주소선언//
	struct sockaddr_in clnt_adr;// 클리이언트 주소선언//
	socklen_t clnt_adr_sz; //주소 길이// 

	if(argc!=2)//실행하려는파일명 포트//
	{
		printf("usage:%s <port>\n,argv[0]");
		exit(1);
	}

	serv_sock=socket(PF_INET,SOCK_STREAM,0);//
	if(serv_sock==-1)//서버소켓이 안열렸을때
		error_handling("socket() error");
	//주소를 초기화시키기//
	memset(&serv_adr,0,sizeof(serv_adr));//서버주소 초기화//
	serv_adr.sin_family=AF_INET;//ip4 를 주소 체계설정//
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);//ip주소 할당 
	serv_adr.sin_port=htons(atoi(argv[1]));//port 번호 초기화
	

	if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)//서버주소가 없을떄//
		error_handling("bind() error");
	if(listen(serv_sock,5)==-1)//대기큐 5 서버 주소가 반환이 안될을때// 
		error_handling("listen() error");

	clnt_adr_sz=sizeof(clnt_adr);//
	for (int i = 0; i < 5; i++)
	{
		clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_adr,&clnt_adr_sz);//연결요청//
		if(clnt_sock==-1)//클라이언트가 잘 안들어졌거나 연결 불가//
			error_handling("accept() error");
		else
			printf("connected client %d\n",i+1);
		while((str_len=read(clnt_sock,message,BUF_SIZE))!=0)
			write(clnt_sock,message,str_len);
			close(clnt_sock);
	}
	
		close(serv_sock);//서버소켓 종료//
		return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}
