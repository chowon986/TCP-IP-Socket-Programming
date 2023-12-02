#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

void error_handling(char* message);

int main(void)
{
    int fildes;
    FILE* fp;

    // 시스템 함수를 통한 파일 생성
    // 시스템 함수를 사용했으므로 파일 디스크립터가 리턴됨
    fildes = open("data.dat", O_WRONLY|O_CREAT|O_TRUNC);
    if(fildes == -1)
        error_handling("file open error");
    
    // 파일 디스크립터를 이용하여 파일 포인터 생성
    fp = fdopen(fildes, "w");

    fputs("Network C programming\n\n", fp);
    // 파일 포인터를 사용해서 표준 함수 fclose 호출
    // 파일 자체가 완전히 닫히기 때문에 파일 디스크립터를 이용한 종료를 다시 해 줄 필요 없음
    fclose(fp);

    return 0;
}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}