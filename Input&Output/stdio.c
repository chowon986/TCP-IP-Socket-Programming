// stdio.c

#include <stdio.h>
#include <stdlib.h>

void error_handling(char* message);

int main(void)
{
	FILE* fp;

	// 표준 입/출력 함수를 통한 파일 생성
	fp = fopen("test.dat", "w");
	if(fp == NULL)
		error_handling("file open error");

	// 파일 포인터를 사용하여 데이터를 파일로 전송
	fputs("Network programming\n\n", fp);
	fclose(fp);
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}