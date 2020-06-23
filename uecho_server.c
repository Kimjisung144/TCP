#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])//argc 
{
	int serv_sock;//서버 소켓//
	socklen_t clnt_adr_sz;
	char message[BUF_SIZE];
	int str_len;

	struct sockaddr_in serv_adr,clnt_adr;// 클리이언트 주소선언//

	if(argc!=2)//실행하려는파일명 포트//
	{
		printf("usage:%s <port>\n,argv[0]");
		exit(1);
	}

	serv_sock=socket(PF_INET,SOCK_DGRAM,0);
	if(serv_sock==-1)//서버소켓이 안열렸을때
		error_handling("UDP socket creation error");
	//주소를 초기화시키기//
	memset(&serv_adr,0,sizeof(serv_adr));//서버주소 초기화//
	serv_adr.sin_family=AF_INET;//ip4 를 주소 체계설정//
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);//ip주소 할당 
	serv_adr.sin_port=htons(atoi(argv[1]));//port 번호 초기화

	if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)//서버주소가 없을떄//
		error_handling("bind() error");

	while(1)
	{
		clnt_adr_sz = sizeof(clnt_adr);
		str_len = recvfrom(serv_sock,message,BUF_SIZE,0,(struct sockaddr*)&clnt_adr,&clnt_adr_sz);
		sendto(serv_sock,message,str_len,0,(struct sockaddr*)&clnt_adr,clnt_adr_sz);		
	}
	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}
