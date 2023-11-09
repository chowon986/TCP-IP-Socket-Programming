#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>


#define BUFSIZE 30

void error_handling(char* message);

int main(int argc, char** argv)
{
    int serv_sd;
    int clnt_sd;
    int fd;
    char buf[BUFSIZE];

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    int clnt_addr_size;
    int len;
    
    if(argc!=2)
    {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    // 접속해 오는 클라이언트에게 전송해 줄 파일 오픈 (읽기 모드)
    fd = open("file_server.c", O_RDONLY);
    if(fd == -1)
        error_handling("File open error");
    
    serv_sd = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sd == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");
    
    if(listen(serv_sd, 5) == -1)
        error_handling("listen() error");
    
    clnt_addr_size = sizeof(clnt_addr);
    clnt_sd = accept(serv_sd, (struct sockaddr*)&clnt_addr, &clnt_addr_size);

    if(clnt_sd == -1)
        error_handling("accept() error");
    
    // 클라이언트에 데이터 전송
    // 오픈한 파일에 있는 내용을 파일의 끝까지 읽어서 스트림을 통해
    // 해당 클라이언트에게 전송해줌
    // read 함수는 파일의 끝에 도달하게 되면 0을 리턴함
    while((len = read(fd, buf, BUFSIZE)) != 0)
        write(clnt_sd, buf, len);
    
    // 데이터 전송 후 소켓의 일부(전송영역)를 닫음
    // 클라이언트에게 EOF 메시지를 보내서 파일 전송이 끝났음을 알려줌
    if(shutdown(clnt_sd, SHUT_WR) == -1)
        error_handling("shutdown error");
    
    // 인사의 메시지를 수신 (입력 스트림은 열려 있으므로)
    len = read(clnt_sd, buf, BUFSIZE);
    write(1, buf, len);

    close(fd);
    close(clnt_sd);

    return 0;
}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}