#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char** argv)
{
	pid_t pid;
	int data = 10;

	pid = fork(); // 자식 프로세스 생성

	if(pid < 0)
        printf("fork 실패 프로세스 id : %d\n", pid);
    
    printf("fork 성공 프로세스 id : %d\n", pid);

    if(pid == 0)
        data += 10;
	else
	{
		data -= 10; // 부모 프로세스에 의해 실행됨
		sleep(20);  // 20초 동안 대기 상태로 둔 이유는 그 사이에 자식 프로세스가 좀비가 된 것을 확인해 보기 위해
	}

	printf("data : %d \n", data);
	return 0;
}