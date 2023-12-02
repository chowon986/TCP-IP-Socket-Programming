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
    char buf[BUFSIZE];

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
        error_handling("listen error");

    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);

    if(clnt_sock == -1)
        error_handling("accept() error");

    rstrm = fdopen(clnt_sock, "r");

    wstrm = fdopen(dup(clnt_sock), "w");

    fputs("FROM SERVER : Hello?\n", wstrm);
    fputs("I like network programming\n", wstrm);
    fputs("I like socket programming\n\n", wstrm);
    fflush(wstrm);

    shutdown(fileno(wstrm), SHUT_WR);
    fclose(wstrm);

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