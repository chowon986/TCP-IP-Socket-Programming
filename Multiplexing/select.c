// select.c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdlib.h>

#define BUFSIZE 30

int main(int argc, char** argv)
{
	fd_set reads, temps;
	int result;

	char message[BUFSIZE];
	int str_len;
	struct timeval timeout;

	FD_ZERO(&reads);   // fd_set 구조체 변수를 초기화
	FD_SET(0, &reads); // standard input 설정 
										 // (파일 디스크립터 0을 나타내는 위치를 1로 설정 -> 표준 입력에 변화가 있는지 관심을 두고 볼 거임)

	/* 여기서 타임아웃 설정을 하면 안 됨
	timeout.tv_sec = 5;
	timeout.tv_usec = 100000;
	*/

	while(1)
	{
		temps = reads; // fd_set 변수를 임시 변수에 복사함
									 // 변화가 생긴 파일 디스크립터 위치를 제외한 나머지 위치의 비트들이 0으로 초기화되므로
								   // 원본 변수를 직접 select 함수의 인자로 전달해버리면 또 다시 변수를 설정해야 함
									 // 이를 막기 위해 원본은 보존하고 임시 변수에 원본을 복사해서 select() 호출
		
		// 타임아웃 설정은 while문 안에서 해야 함 -> 매번 타임아웃을 재설정 할 수 있어야 하므로
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;

		result = select(1, &temps, 0, 0, &timeout);
		if(result == -1)
		{
			puts("select 오류 발생");
			exit(1);
		}
		else if(result == 0)
		{
			puts("시간이 초과되었습니다 : select ");
		}
		else
		{
			if(FD_ISSET(0, &temps))
			{
				str_len = read(0, message, BUFSIZE);
				message[str_len] = 0;
				fputs(message, stdout);
			}
		}
	}
}