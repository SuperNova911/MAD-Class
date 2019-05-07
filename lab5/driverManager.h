#include <stdbool.h>


bool openDriver();
void closeDriver();

bool setLED(unsigned char value);
unsigned char getLED();
void clearLED();

size_t getSwitchStatus();
bool isSwitchOn(int switchNumber);

bool setDotMatrix(unsigned char font[], size_t length);
bool setDotMatrixByNumber(int value);
void clearDotMatrix();
void setAllDotMatrix();
