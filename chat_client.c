#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <semaphore.h>

#define BUF_SIZE 100
#define NAME_SIZE 20
void * send_msg(void * arg);
void * recv_msg(void * arg);
void error_handling(char * msg);

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];
int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	pthread_t snd_thread, rcv_thread;
	void * thread_return;
	if(argc!=4)//ip sock port//
	{
		printf("Usage: %s <IP> <PORT> <name>\n",argv[0]);
		exit(1);
	}
	sprintf(name,"[%s]",argv[3]);
	sock = socket(PF_INET,SOCK_STREAM,0);

	memset(&serv_addr,0,sizeof(serv_addr));//서버주소 초기화//
	serv_addr.sin_family=AF_INET;//ip4 를 주소 체계설정//
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);//ip주소 할당 
	serv_addr.sin_port=htons(atoi(argv[2]));//port 번호 초기화

	if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)//연결요청 동시에 서버에 클라이언트 주소 전송
		error_handling("connect() error");
	pthread_create(&snd_thread,NULL,send_msg,(void*)&sock);
	pthread_create(&rcv_thread,NULL,recv_msg,(void*)&sock);
	pthread_join(snd_thread,&thread_return);
	pthread_join(rcv_thread,&thread_return);
	close(sock);
	return 0;
}
void * send_msg(void *arg)
{
	int sock=*((int*)arg);
	char name_msg[NAME_SIZE+BUF_SIZE];
	printf("INPUT MESSAGE:");
	while(1)
	{
		fgets(msg,BUF_SIZE,stdin);
		if(!strcmp(msg,"q\n")||!strcmp(msg,"Q\n"))
		{
			close(sock);
			exit(0);
		}
		sprintf(name_msg,"%s:%s input message:\n",name,msg);
		write(sock,name_msg,strlen(name_msg));
	}
	return NULL;
}
void * recv_msg(void *arg)
{
	int sock=*((int*)arg);
	char name_msg[NAME_SIZE+BUF_SIZE];
	int str_len;
	while(1)
	{
		str_len=read(sock,name_msg,NAME_SIZE+BUF_SIZE-1);
		if(str_len==-1)
			return (void*)-1;
		name_msg[str_len]=0;
		fputs(name_msg,stdout);
	}
	return NULL;
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}
