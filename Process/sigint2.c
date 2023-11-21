#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void handler(int sig);

int main(void)
{
    int state;
    int num = 0;

    struct sigaction act;
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    state = sigaction(SIGINT, &act, 0);
    if(state != 0)
    {
        puts("sigaction() error ");
        exit(1);
    }

    while(1)
    {
        printf("%d : 대기중 \n", num++);
        sleep(2);
        if (num > 5)
            break;
    }
    return 0;
}

void handler(int sig)
{
    printf("전달된 시그널은 %d \n", sig);
}