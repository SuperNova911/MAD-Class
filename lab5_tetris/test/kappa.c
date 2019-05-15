#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "driverManager.h"

int main()
{
    int index = 0;

    if (openDriver() == false)
    {
        return -1;
    }

    // LED Test
    printf("Set/Get LED 0 ~ 255\n");
    for (index = 0; index <= LED_MAX; index++)
    {
        usleep(50000);
        setLED((unsigned char)index);
        printf("LED Value: '%d'\n", getLED());
    }

    printf("Clear LED\n");
    usleep(10000000);
    clearLED();

    // DotMatrix Test
    printf("Set all DotMatrix\n");
    usleep(10000000);
    setAllDotMatrix();

    printf("Clear DotMatrix\n");
    usleep(10000000);
    clearDotMatrix();

    printf("Set DotMatrix by font\n");
    for (index = 0; index < 10; index++)
    {
        usleep(1000000);
//        setDotMatrix(dot_matrix_font[index], sizeof(dot_matrix_font[index]));
        printf("DotMatrix set to: '%d'\n", index);
    }

    printf("Set DotMatrix by number\n");
    for (index = 0; index < 10; index++)
    {
        usleep(100000);
        setDotMatrixByNumber(index);
        printf("DotMatrix set to: '%d'\n", index);
    }

    closeDriver();

    return 0;
}
