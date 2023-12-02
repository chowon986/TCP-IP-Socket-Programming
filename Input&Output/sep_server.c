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
    int serv_sock;
    int clnt_sock;

    FILE* rstrm;
    FILE* wstrm;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    int clnt_addr_size;

    char buf[BUFSIZE] = {0,};

    if(argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");
    
    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");
    
    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct  sockaddr*)&clnt_addr, &clnt_addr_size);
    if(clnt_sock == -1)
        error_handling("accept() error");
    
    // 입력 스트림과 출력 스트림 생성
    rstrm = fdopen(clnt_sock, "r");
    wstrm = fdopen(clnt_sock, "w");

    fputs("FROM SERVER : Hello?\n", wstrm);
    fputs("I like network programming\n", wstrm);
    fputs("I like socket programming\n\n", wstrm);
    fflush(wstrm);

    // EOF 메시지 전송을 위해 출력용 파일포인터를 이용하여
    // 출력 스트림을 종료하고 있음
    // 그러나 실제로는 해당 소켓도 종료가 되기 때문에
    // 입/출력 스트림 모두 종료되어 버림
    fclose(wstrm);

    // 입력 스트림으로부터 전달되는 데이터를 수신하려 하고 있지만
    // 소켓이 닫혀진 상태이므로 데이터를 더 이상 수신할 수 없게 됨
    fgets(buf, sizeof(buf), rstrm);
    fputs(buf, stdout);


    fclose(rstrm);
    return 0;
}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}