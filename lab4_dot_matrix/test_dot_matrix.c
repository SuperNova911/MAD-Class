#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>

#include "dot_matrix_font.h"

int main(int argc, char * argv[])
{
    int fd;
    int input;

    if (argc < 2 || atoi(argv[1]) < 0 || atoi(argv[1]) > 9)
    {
        printf("0~9 사이의 정수를 입력하세요.\n");
        return -1;
    }

    input = atoi(argv[1]);

    fd = open("/dev/csemad_dot_matrix", O_RDWR);

    if (fd < 0)
    {
        printf("Failed to open DotMatrix driver, result: '%d'\n", fd);
        return -1;
    }

    write(fd, dot_matrix_font[input], sizeof(dot_matrix_font[input]));

    close(fd);
    return 0;
}
