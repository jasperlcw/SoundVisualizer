#include <stdio.h>
#include <stdlib.h>
#include "Utility.h"
#include <pthread.h>


#include "include/potentiometer.h"
#include "include/ledControl.h"

static bool isRunning;

// Potentiometer
#define A2D_FILE_POTENTIOMETER "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"
#define A2D_MAX_READING 4095

static void *potThread(void *vargp);

static pthread_t potThreadId;

int PotReading = 0;

//start the Pot thread
void Potstart(){
    isRunning = true;
    pthread_create(&potThreadId, NULL, potThread, NULL);
}
//ends the Pot thread
void Potcleanup(){
    isRunning = false;
    pthread_join(potThreadId,NULL);
   
}

void *potThread(void *vargp){
    while (isRunning)
    {
        PotReading = getVoltageReading(A2D_FILE_POTENTIOMETER);
        sleepForMs(100);
    }
    return NULL;
    
}

// From potDriver.c
int getVoltageReading(char* path)
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

int getPotValue(){
    return PotReading;
}

// Returns a value from 0 - 100 (inclusive) representing the percentage
// of the way the potentiometer has been turned
int Potentiometer_getReading()
{
    double percentTurned = (double)PotReading / (double)A2D_MAX_READING;
    return (int)(percentTurned * 100);
}


int Potentiometer_potToColor()
{
    // We have 8 LED settings, and a reading of 0 - 99
    double partition = 100 / 8;
    int reading = Potentiometer_getReading();

    if (reading < partition * 1) {
        return LED_BLACK;
    } else if (reading < partition * 2) {
        return LED_RED;
    } else if (reading < partition * 3) {
        return LED_GREEN;
    } else if (reading < partition * 4) {
        return LED_YELLOW;
    } else if (reading < partition * 5) {
        return LED_BLUE;
    } else if (reading < partition * 6) {
        return LED_MAGENTA;
    } else if (reading < partition * 7) {
        return LED_TEAL;
    } else {
        return LED_WHITE;
    }
} 
