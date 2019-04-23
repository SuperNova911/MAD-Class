#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char * argv[])
{
	int fd;
	unsigned char input;
    unsigned char currentLED;

    if (argc < 2 || atoi(argv[1]) < 0 || atoi(argv[1]) > 255)
	{
		printf("0~255 사이의 값을 입력하세요.\n");
		return -1;
	}

	input = (unsigned char)atoi(argv[1]);

	fd = open("/dev/csemad_led", O_RDWR);

	if (fd < 0)
	{
		printf("Failed to open LED Driver, result: '%d'\n", fd);
		return -1;
	}

	write(fd, &input, 1);
	read(fd, &currentLED, 1);

	printf("Current LED Value: '%d'\n", currentLED);

	close(fd);
    return 0;
}
