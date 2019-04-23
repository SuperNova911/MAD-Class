#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>

#define MAX_BUTTON 9

unsigned char Quit = 0;

void user_signal1(int sig)
{
    Quit = 1;
}

int main(void)
{
    int fd;
    int index;

    unsigned char pushSwitchBuffer[MAX_BUTTON];

    fd = open("/dev/csemad_push_switch", O_RDWR);

    if (fd < 0)
    {
        printf("Failed to open PushSwitch driver, result: '%d'\n", fd);
        return -1;
    }

    (void)signal(SIGINT, user_signal1);

    printf("Press <ctrl+c> to quit.\n");

    while (!Quit)
    {
        usleep(400000);

        read(fd, pushSwitchBuffer, sizeof(pushSwitchBuffer));
        for (index = 0; index < MAX_BUTTON; index++)
        {
            printf("'%d:%d' ", index + 1, pushSwitchBuffer[index]);
        }
        printf("\n");
    }

    close(fd);
    return 0;
}
