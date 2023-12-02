#include <stdio.h>
#include <unistd.h>

int main(void)
{
    int fd;
    fd = dup(1); // 파일 디스크립터 복사
    printf("복사된 파일 디스크립터 : %d\n", fd);
    write(fd, "복사된 파일 디스크립터에 의한 출력 \n", 72);

    return 0;
}