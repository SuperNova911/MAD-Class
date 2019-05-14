#include "driverManager.h"
#include "tetris.h"

#include <stdio.h>
#include <stdlib.h>

int main()
{
    openDriver();

    InitializeTetris();



    closeDriver();

    return 0;
}
