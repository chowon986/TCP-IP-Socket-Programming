#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

void error_handling(char* message);

int main(int argc, char** argv)
{
	int tcp_socket;
	int udp_socket;

	// 연결 지향 TCP 소켓 생성
	tcp_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(tcp_socket == -1)
		error_handling("socket() error");

	udp_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(udp_socket == -1)
		error_handling("socket() error");

	printf("생성된 TCP 소켓의 파일 디스크립터 : %d \n", tcp_socket);
	printf("생성된 UDP 소켓의 파일 디스크립터 : %d \n", udp_socket);

	close(tcp_socket);
	close(udp_socket);
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}