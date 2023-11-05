#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

void error_handling(char* message);

int main(int argc, char** argv)
{
	int serv_sock;
	int clnt_sock;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	int clnt_addr_size;
	char message[]="Hello World!\n";
	
	if(argc!=2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock=socket(PF_INET, SOCK_STREAM, 0); // 서버 소켓 생성
	if(serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET; // 체계
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY); // 자동으로 ip주소 가져와서 네트워크 long 주소로 변환
	serv_addr.sin_port=htons(atoi(argv[1])); // argv[1]에 포트 번호가 있으니 가져와서 네트워크 주소로 변환

	if(bind(serv_sock, (struct sockaddr*) &serv_addr,
			sizeof(serv_addr)) == -1) // 소켓에 주소 할당
		error_handling("bind() error");

	if(listen(serv_sock, 5) == -1) // 연결 요청 대기 상태로 진입
		error_handling("listen() error");

	clnt_addr_size = sizeof(clnt_addr);

	for( ; ; )
	{
        // 연결 요청 수락
		clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
	
		if(clnt_sock == -1)
		{
			error_handling("accept() error");
			break;
		}
		
			// 데이터 전송 후 클라이언트 연결 소켓의 연결 종료
			write(clnt_sock, message, sizeof(message));
			close(clnt_sock);
	}

	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}