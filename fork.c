#include <stdio.h>
#include <unistd.h>

int gval=10;
int main(int argc, char *argv[])
{
	pid_t pid;
	int ival = 20;
	gval++,ival+=5;//gval=11,ival=25//
	pid=fork();//자식 프로세서 생성
	if(pid==0)
			gval+=2,ival+=2;
		else
			gval-=2,ival-=2;
	if(pid==0)
		printf("child proc:[%d, %d]\n",gval,ival);
		else
			printf("parent proc:[%d,%d]\n",gval,ival);
				return 0;
}