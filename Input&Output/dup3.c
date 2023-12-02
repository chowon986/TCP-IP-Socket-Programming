#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
    int fd;
    int state;
    fd = dup(1); // standard out 복사
    printf("복사된 파일 디스크립터 : %d\n", fd);
    write(fd, "복사된 파일 디스크립터에 의한 출력 \n", 52);

    if(close(1) == -1)
    {
        puts("에러 발생 \n");
        exit(1);
    }

    write(fd, "복사된 파일 디스크립터에 의한 출력 \n", 52);

    if(close(fd) == -1)
    {
        puts("에러 발생 \n");
        exit(1);
    }

    write(fd, "복사된 파일 디스크립터에 의한 출력 \n", 52);
}