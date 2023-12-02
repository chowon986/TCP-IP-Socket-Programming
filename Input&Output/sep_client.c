#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFSIZE 1024
void error_handling(char* message);

int main(int argc, char** argv)
{
    int sock;

    FILE* rstrm;
    FILE* wstrm;
    char buf[BUFSIZE];

    struct sockaddr_in serv_addr;

    if(argc != 3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect error!");
    
    // 입력 스트림과 출력 스트림 생성
    rstrm = fdopen(sock, "r");
    wstrm = fdopen(sock, "w");

    // EOF 메시지가 들어올 때까지 데이터를 읽어서 콘솔에 출력해 주고 있음
    while(1)
    {
        // fgets 함수는 EOF 수신 시 NULL 포인터가 리턴됨
        if(fgets(buf, sizeof(buf), rstrm) == NULL) break;
        fputs(buf, stdout);
        fflush(stdout);
    }

    // 인사 메시지 전송
    fputs("FROM CLIENT : Thanks you!\n", wstrm);
    fflush(wstrm);

    fclose(wstrm);
    fclose(rstrm);
    return 0;    
}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}