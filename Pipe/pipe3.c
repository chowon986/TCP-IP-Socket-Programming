#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFSIZE 30

int main(int argc, char** argv)
{
	int fd1[2], fd2[2];
	char buffer[BUFSIZE];
	pid_t pid;

	if(pipe(fd1) == -1 || pipe(fd2) == -1)
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
		write(fd1[1], "Good!", 6); // 데이터를 전송하기 위해 사용
		read(fd2[0], buffer, BUFSIZE); // 데이터를 수신하기 위해 사용
		printf("자식 프로세스 출력 : %s \n\n", buffer);
	}
	else
	{
		read(fd1[0], buffer, BUFSIZE); // 데이터를 수신하기 위해 사용
		printf("부모 프로세스 출력 : %s \n", buffer); // 데이터를 전송하기 위해 사용
		write(fd2[1], "Really Good", 12);
		sleep(1);
	}

	return 0;
}