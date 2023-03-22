#include <stdio.h>
#include <stdlib.h>

#include "include/potentiometer.h"

// Potentiometer
#define A2D_FILE_POTENTIOMETER "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"

#define A2D_MAX_READING 4095

// From potDriver.c
static int getVoltageReading(char* path)
{
    // Open file
    FILE *f = fopen(path, "r");
    if (!f) {
        printf("ERROR: Unable to open voltage input file. Cape loaded?\n");
        printf(" Check /boot/uEnv.txt for correct options.\n");
        exit(-1);
    }
    // Get reading
    int a2dReading = 0;
    int itemsRead = fscanf(f, "%d", &a2dReading);
    if (itemsRead <= 0) {
        printf("ERROR: Unable to read values from voltage input file.\n");
		exit(-1);
    }
    // Close file
    fclose(f);
    return a2dReading;
}

// Returns a value from 0 - 100 (inclusive) representing the percentage
// of the way the potentiometer has been turned
int Potentiometer_getReading()
{
    double potReading = getVoltageReading(A2D_FILE_POTENTIOMETER);
    double percentTurned = potReading / A2D_MAX_READING;
    return percentTurned * 100;
}
