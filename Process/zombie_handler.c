#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void z_handler(int sig);

int main(int argc, char** argv)
{
	pid_t pid;
	int state, i;

	struct sigaction act;
	act.sa_handler = z_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	// 시그널 핸들러 등록
	// SIGCHLD는 자식 프로세스가 종료됐을 때 발생하는 시그널
	state = sigaction(SIGCHLD, &act, 0); 

	if(state != 0)
	{
		puts("sigaction() error ");
		exit(1);
	}

	pid = fork();

	if(pid == 0)
	{
		// getpid()는 현재 실행되고 있는 프로세스의 ID를 리턴해줌
		// 자식 프로세스에 의해 실행되는 영역이므로 자식 프로세스의 ID를 출력하게 됨
		printf("자식 프로세스 생성 : %d \n", getpid());
		// 자식 프로세스가 종료하면서 3 반환
		exit(3);
	}
	else
		// 자식 프로세스가 부모 프로세스보다 먼저 종료될 수 있게 부모 프로세스의 종료는 3초 늦춤
		sleep(3);

	return 0;
}

void z_handler(int sig)
{
	pid_t pid;
	int rtn;
	
	// waitpid를 통해 자식 프로세스의 리턴값을 읽어들이고 있음
	while( (pid = waitpid(-1, &rtn, WNOHANG) ) > 0 )
	{
		// 자식 프로세스의 리턴 값으로 3을 넣어줬으니 이 안으로 들어옴
		// 여러 개의 자식 프로세스가 종료되는 상황을 염려해서 while 문 안에서 반복적으로 처리해 줌
		printf("소멸된 좀비의 프로세스 ID : %d \n", pid);
		printf("리턴된 데이터 : %d \n\n", WEXITSTATUS(rtn));
	}
}