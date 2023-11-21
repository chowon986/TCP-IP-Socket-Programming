#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    pid_t pid, child;
    int data = 10;
    int state;

    pid = fork();
    if (pid < 0)
        printf("fork 실패 프로세스 id : %d\n", pid);
    
    printf("fork 성공 프로세스 id : %d\n", pid);

    if (pid == 0)
        data += 10;
    else
    {
        data -= 10;
        child = wait(&state);
        printf("자식 프로세스 ID = %d\n", child);
        printf("리턴 값 = %d\n", WEXITSTATUS(state));
        sleep(20);
    }

    printf("data : %d\n", data);
    return 0;
}