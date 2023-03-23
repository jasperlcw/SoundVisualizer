#include "../include/ledControl.h"

#include <stdio.h>
#include <stdlib.h>

/*
*   Note: This program will never terminate by itself
*   Termination is done external to the application with Ctrl-C
*/
int main()
{   

    LED_init();
    LED_wait();
    return 0;
}


