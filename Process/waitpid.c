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
        printf("fork 실패, 프로세스 id : %d\n", pid);
    
    printf("fork 성공, 프로세스 id : %d\n", pid);

    if(pid == 0)
    {
        data += 10;
        sleep(10);
    }
    else
    {
        data -= 10;
        do
        {
            sleep(3);
            puts("3초 대기");
            child = waitpid(-1, &state, WNOHANG);
        } while (child == 0);
        
        printf("Child process id = %d, return value = %d \n\n", child, WEXITSTATUS(state));
    }

    printf("data : %d\n", data);
    return 0;
}