// inet_ntoa.c

#include <stdio.h>
#include <arpa/inet.h>

int main(void)
{
	struct sockaddr_in addr1, addr2;
	char* str;

	addr1.sin_addr.s_addr = htonl(0x1020304);
	addr2.sin_addr.s_addr = htonl(0x1010101);

	str = inet_ntoa(addr1.sin_addr);
	printf("Dotted-Decimal notation : %s \n", str);

	inet_ntoa(addr2.sin_addr); // inet_ntoa의 리턴 값을 str에 넣어주지 않았으나
	printf("Dotted-Decimal notation : %s \n\n", str); // 출력 결과를 보면 다른 결과가 나옴
	return 0;
}