#include "../include/ledControl.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

static void sleepForMs(long long delayInMs);

/*
*   Note: This program will never terminate by itself
*   Termination is done external to the application with Ctrl-C
*/
int main()
{   

    LED_init(LED_LISTEN);

    // Wait 6 seconds for the init process to finish
    sleepForMs(5000);

    // Display a left to right sweep effect using LED_setDisplay
    int matrix[16][32] = {0};
    for (int colNum = 0; colNum < 32; colNum++) {
        for (int rowNum = 0; rowNum < 16; rowNum++) {
            matrix[rowNum][colNum] = LED_WHITE;
        }
        LED_setDisplay(16, 32, matrix);
        memset(matrix, 0, sizeof(matrix));
        sleepForMs(50);
    }
    LED_setDisplay(16, 32, matrix);

    // Switch mode to display current time
    LED_setMode(LED_CLOCK);
    LED_wait();
    return 0;
}

static void sleepForMs(long long delayInMs)
{
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;
    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;
    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *)NULL);
}
