#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFSIZE 100

void error_handling(char* message);

int main(void)
{
	int fildes;
	char buf[BUFSIZE];

	fildes = open("data.txt", O_RDONLY);
	if(fildes == -1)
		error_handling("open() error!");

	printf("오픈 한 파일의 파일 디스크립터는 %d 입니다.\n", fildes);

	if(read(fildes, buf, sizeof(buf)) == -1)
		error_handling("read() error!");

	printf("파일이 내용 : %s", buf);

	close(fildes);
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputs("\n", stderr);
	exit(1);
}