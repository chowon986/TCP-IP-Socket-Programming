// echo_selserv.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFSIZE 100
void error_handling(char* message);

int main(int argc, char** argv)
{
	int serv_sock;
	struct sockaddr_in serv_addr;

	fd_set reads, temps;
	int fd_max;

	char message[BUFSIZE];
	int str_len;
	struct timeval timeout;

	if(argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))
		error_handling("bind() error");
	if(listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	FD_ZERO(&reads);
	FD_SET(serv_sock, &reads); // fd_set 변수에 서버 소켓의 파일 디스크립터를 설정해 줌
														 // 서버 소켓으로부터 수신할 데이터가 있는지 관심을 두겠다는 의미
	fd_max = serv_sock;

	while(1) 
	{
		int fd, str_len;
		int clnt_sock, clnt_len;
		struct sockaddr_in clnt_addr;
		
		temps = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;

		if(select(fd_max + 1, &temps, 0, 0, &timeout) == -1)
			error_handling("select() error");

		for(fd = 0; fd < fd_max + 1; fd++)
		{
			// select 함수 호출 후에 상태 변화가 있었던 파일 디스크립터를 찾아내기
			if(FD_ISSET(fd, &temps))
			{
				if(fd == serv_sock) // 서버 소켓인지 확인하기 (서버 소켓에서 변화가 있었다 -> 연결 요청이 있었다를 의미)
				{
					clnt_len = sizeof(clnt_addr);
					clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_len); // 연결 요청 수락
					FD_SET(clnt_sock, &reads); // 리턴되는 소켓을 reads 변수에 추가함 (모든 클라이언트의 입/출력을 하나로 멀티플렉싱 하기 위해)
					if(fd_max < clnt_sock)
						fd_max = clnt_sock;
					printf("클라이언트 연결 : 파일 디스크립터 %d\n", clnt_sock);
				}
				else // 변화가 있던 파일 디스크립터가 서버 소켓이 아닌 경우 (클라이언트로부터 데이터가 전송된 경우 - 데이터 또는 연결 종료)
				{
					str_len = read(fd, message, BUFSIZE);
					if(str_len == 0) // 연결 종료인지 확인
					{
						FD_CLR(fd, &reads); // reads 변수에서 제외
						close(fd);
						printf("클라이언트 종료 : 파일 디스크립터 %d\n", fd); 
					}
					else
						write(fd, message, str_len);
				}
			}
		}
	}
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}