#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

# define BUFSIZE 30

int main(int argc, char** argv)
{
	int fd[2];
	char buffer[BUFSIZE];
	pid_t pid;
	int state;

	state = pipe(fd); // 파이프를 생성하고 있음
	if(state == -1)
	{
		puts("pipe() error");
		exit(1);
	}

    pid = fork();
    if(pid == -1)
    {
        puts("fork() error");
        exit(1);
    }
	else if(pid == 0)
	{
		write(fd[1], "Good!", 6);
		// 여기를 주석 처리하고 실행하면, 자식 프로세스는 파이프를 통해서 데이터를 전송 하자마자,
		// 바로 다음 줄에서 파이프로부터 데이터를 수신해버림
		// 즉, 자식 프로세스는 데이터를 파이프로 전송하고 나서 본인이 가져가 버림
		sleep(2);
		read(fd[0], buffer, BUFSIZE);
		printf("자식 프로세스 출력 : %s \n\n", buffer);
	}
	else
	{
		// 자식 프로세스의 sleep이 주석이 되면 부모 프로세스는 데이터가 들어오기를 기다림(블로킹 상태)
		read(fd[0], buffer, BUFSIZE);
		printf("부모 프로세스 출력 : %s \n", buffer);
		write(fd[1], "Really Good", 12);
		sleep(3);
	}

	return 0;
}