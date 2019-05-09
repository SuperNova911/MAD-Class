#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "dot_matrix_font.h"
#include "driverManager.h"

const unsigned char LED_CLEAR_VALUE = 0;

const int PUSH_SWITCH_NUMBER = 9;

int LEDFD;
int PushSwitchFD;
int DotMatrixFD;

// Open, Close Drivers
bool openDriver()
{
    LEDFD = open("/dev/csemad_led", O_RDWR);
    if (LEDFD < 0)
    {
        printf("Failed to open LED driver, result: '%d'\n", LEDFD);
        return false;
    }

    PushSwitchFD = open("/dev/csemad_push_switch", O_RDONLY);
    if (PushSwitchFD < 0)
    {
        printf("Failed to open PushSwitch driver, result: '%d'\n", PushSwitchFD);
        return false;
    }

    DotMatrixFD = open("/dev/csemad_dot_matrix", O_WRONLY);
    if (DotMatrixFD < 0)
    {
        printf("Failed to open DotMatirx driver, result: '%d'\n", DotMatrixFD);
        return false;
    }

    return true;
}

void closeDriver()
{
    close(LEDFD);
    close(PushSwitchFD);
    close(DotMatrixFD);
}

// LED Management
bool setLED(unsigned char value)
{
    if (write(LEDFD, &value, sizeof(value)) != sizeof(value))
    {
        printf("Failed to set LED, value: '%d'\n", value);
        return false;
    }

    return true;
}

unsigned char getLED()
{
    signed char currentValue = 0;

    if (read(LEDFD, &currentValue, sizeof(currentValue)) != sizeof(currentValue))
    {
        printf("Failed to get LED value\n");
    }

    return currentValue;
}

void clearLED()
{
    if (write(LEDFD, &LED_CLEAR_VALUE, sizeof(LED_CLEAR_VALUE)) != sizeof(LED_CLEAR_VALUE))
    {
        printf("Failed to clear LED\n");
    }
}

// PushSwitch Management
size_t getSwitchStatus(unsigned char statusBuffer[])
{
    ssize_t readResult;
    readResult = read(PushSwitchFD, statusBuffer, sizeof(statusBuffer) * PUSH_SWITCH_NUMBER);

    if (readResult != sizeof(statusBuffer) * PUSH_SWITCH_NUMBER)
    {
        printf("Failed to get PushSwitch status, read: '%d'bytes\n", readResult);
    }

    return readResult;
}

bool isSwitchOn(int switchNumber)
{
    unsigned char statusBuffer[PUSH_SWITCH_NUMBER];

    if (getSwitchStatus(statusBuffer) != PUSH_SWITCH_NUMBER)
    {
        printf("Cannot get PushSwitch status\n");
        return false;
    }

    return statusBuffer[switchNumber] ? true : false;
}

// DotMatrix Management
bool setDotMatrix(unsigned char font[], size_t length)
{
    // TODO: Implement code
    return false;
}

bool setDotMatrixByNumber(int value)
{
    if (value < 0 || value > 9)
    {
        printf("Input value must be 0~9, value: '%d'\n", value);
        return false;
    }

    if (write(DotMatrixFD, dot_matrix_font[value], sizeof(dot_matrix_font[value])) != sizeof(dot_matrix_font[value]))
    {
        printf("Failed to set DotMatrix, value: '%d'\n", value);
        return false;
    }

    return true;
}

void clearDotMatrix()
{
    write(DotMatrixFD, dot_matrix_full, sizeof(dot_matrix_blank));
}

void setAllDotMatrix()
{
    write(DotMatrixFD, dot_matrix_blank, sizeof(dot_matrix_full));
}
