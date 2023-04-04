/*  ledControl.c
*   This file contains implmentation of functionality to update the LED display
*   This code is modified from the provided test code found in the guides of
*   the provided CMPT 433 resources: https://github.com/MontreaI/BeagleBone-Green-Adafruit-16x32-LED-Matrix-Sample-Code/blob/master/test_ledMatrix.c
*/

#include "audioMixer/audioMixer_template.h"
#include "include/currentTime.h"
#include "include/ledControl.h"
#include "include/ledMap.h"
#include "include/potentiometer.h"
#include "Utility.h"

#include <fcntl.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

/* GPIO Pins Definition */
#define RED1_PIN 8     // UPPER
#define GREEN1_PIN 80
#define BLUE1_PIN 78
#define RED2_PIN 76 // LOWER
#define GREEN2_PIN 79
#define BLUE2_PIN 74
#define CLK_PIN 73      // Arrival of each data
#define LATCH_PIN 75    // End of a row of data
#define OE_PIN 71       // Transition from one row to another
#define A_PIN 72        // Row select
#define B_PIN 77
#define C_PIN 70

#define S_IWRITE "S_IWUSR"

/* TIMING */
#define DELAY_IN_US 1500000
#define DELAY_IN_SEC 0

// Number Pixel Size
#define NUMBER_SIZE_HORIZONTAL 5
#define NUMBER_SIZE_VERTICAL 7

/* LED Screen Values */
static int screen[32][16];

/* FILES HANDLER */
static int fileDesc_red1;
static int fileDesc_blue1;
static int fileDesc_green1;
static int fileDesc_red2;
static int fileDesc_blue2;
static int fileDesc_green2;
static int fileDesc_clk;
static int fileDesc_latch;
static int fileDesc_oe;
static int fileDesc_a;
static int fileDesc_b;
static int fileDesc_c;

static pthread_t ledThreadID;
static pthread_mutex_t ledScreenMutex;
static LED_Mode currentMode;
static bool isBoardRunning;
static sem_t ledSem;

// Function for the thread to continuously refresh the LED panel.
static void* ledThread(void *vargp);

// Static helper functions
static void updateClockDisplay(void);

/**
 * exportAndOut
 * Export a pin (if not already exported) and set the direction to output
 * @params
 *  int pinNum: the pin number to be exported and set for output
 */
static void exportAndOut(int pinNumber)
{
    sleepForMs(330);

    char fileNameBuffer[1024];
    sprintf(fileNameBuffer, "/sys/class/gpio/gpio%d/direction", pinNumber);
    FILE *gpioDirP = fopen(fileNameBuffer, "w");

    // Check for whether the pin is exported
    if (gpioDirP == NULL) {
        // If the pointer is NULL, the pin has not been exported, so export it
        FILE *pFile = fopen("/sys/class/gpio/export" , "w");
        if (pFile == NULL) {
            printf("ERROR: Unable to open export file.\n");
            exit(1);
        }
        fprintf(pFile, "%d", pinNumber);
        fclose(pFile);
        sleepForMs(200);

        // Pin should now be exported, so reassign the gpioDirP file pointer
        gpioDirP = fopen(fileNameBuffer, "w"); // okay to be reassigned as it was NULL
    }

    // Now set the direction to out
    fprintf(gpioDirP, "out");
    fclose(gpioDirP);
}

/**
 * ledMatrix_setupPins
 * Setup the pins used by the led matrix, by exporting and set the direction to out
 */
static void ledMatrix_setupPins(void)
{   
    // !Upper led
    exportAndOut(RED1_PIN);
    fileDesc_red1 = open("/sys/class/gpio/gpio8/value", O_WRONLY, S_IWRITE);
    exportAndOut(GREEN1_PIN);
    fileDesc_green1 = open("/sys/class/gpio/gpio80/value", O_WRONLY, S_IWRITE);
    exportAndOut(BLUE1_PIN);
    fileDesc_blue1 = open("/sys/class/gpio/gpio78/value", O_WRONLY, S_IWRITE);

    // Lower led
    exportAndOut(RED2_PIN);
    fileDesc_red2 = open("/sys/class/gpio/gpio76/value", O_WRONLY, S_IWRITE);
    exportAndOut(GREEN2_PIN);
    fileDesc_green2 = open("/sys/class/gpio/gpio79/value", O_WRONLY, S_IWRITE);
    exportAndOut(BLUE2_PIN);
    fileDesc_blue2 = open("/sys/class/gpio/gpio74/value", O_WRONLY, S_IWRITE);

    // Timing
    exportAndOut(CLK_PIN);
    fileDesc_clk = open("/sys/class/gpio/gpio73/value", O_WRONLY, S_IWRITE);
    exportAndOut(LATCH_PIN);
    fileDesc_latch = open("/sys/class/gpio/gpio75/value", O_WRONLY, S_IWRITE);
    exportAndOut(OE_PIN);
    fileDesc_oe = open("/sys/class/gpio/gpio71/value", O_WRONLY, S_IWRITE);

    // Row Select
    exportAndOut(A_PIN);
    fileDesc_a = open("/sys/class/gpio/gpio72/value", O_WRONLY, S_IWRITE);
    exportAndOut(B_PIN);
    fileDesc_b = open("/sys/class/gpio/gpio77/value", O_WRONLY, S_IWRITE);
    exportAndOut(C_PIN);
    fileDesc_c = open("/sys/class/gpio/gpio70/value", O_WRONLY, S_IWRITE); 

    return;
}

/** 
 *  ledMatrix_clock
 *  Generate the clock pins
 */
static void ledMatrix_clock(void)
{
    // Bit-bang the clock gpio
    // Notes: Before program writes, must make sure it's on the 0 index
    lseek(fileDesc_clk, 0, SEEK_SET);
    write(fileDesc_clk, "1", 1);
    lseek(fileDesc_clk, 0, SEEK_SET);
    write(fileDesc_clk, "0", 1);

    return;
}

/**
 *  ledMatrix_latch
 *  Generate the latch pins
 */
static void ledMatrix_latch(void)
{
    lseek(fileDesc_latch, 0, SEEK_SET);
    write(fileDesc_latch, "1", 1);
    lseek(fileDesc_latch, 0, SEEK_SET);
    write(fileDesc_latch, "0", 1);

    return;
}

/**
 *  ledMatrix_bitsFromInt
 *  Convert integer passed on into bits and put in array
 *  @params:
 *      int * arr: pointer to array to be filled with bits
 *      int input: integer to be converted to bits
 */
static void ledMatrix_bitsFromInt(int * arr, int input)
{
    // This function grabs the first three bits of input and sticks into arr where 
    // position 0 is the LSB and position 2 is the MSB
    arr[0] = input & 1;

    arr[1] = input & 2;
    arr[1] = arr[1] >> 1;

    arr[2] = input & 4;
    arr[2] = arr[2] >> 2;

    return;
}

/**
 *  ledMatrix_setRow
 *  Set LED Matrix row
 *  @params:
 *      int rowNum: the rowNumber to be inputted to row pins
 */
static void ledMatrix_setRow(int rowNum)
{
    // This function manipulates pins a, b, and c suggesting those 3 pins are used for row manipulation

    // Convert rowNum single bits from int
    int arr[3] = {0, 0, 0};
    ledMatrix_bitsFromInt(arr, rowNum);

    // Write on the row pins
    char a_val[2];
    sprintf(a_val, "%d", arr[0]);
    lseek(fileDesc_a, 0, SEEK_SET);
    write(fileDesc_a, a_val, 1);

    char b_val[2];
    sprintf(b_val, "%d", arr[1]);
    lseek(fileDesc_b, 0, SEEK_SET);
    write(fileDesc_b, b_val, 1);

    char c_val[2];
    sprintf(c_val, "%d", arr[2]);
    lseek(fileDesc_c, 0, SEEK_SET);
    write(fileDesc_c, c_val, 1);

    return;
}

/**
 *  ledMatrix_setColourTop
 *  Set the colour of the top part of the LED
 *  @params:
 *      int colour: colour to be set
 */
static void ledMatrix_setColourTop(int colour)
{
    int arr[3] = {0, 0, 0};
    ledMatrix_bitsFromInt(arr, colour);

    // Write on the colour pins
    char red1_val[2];
    sprintf(red1_val, "%d", arr[0]);
    lseek(fileDesc_red1, 0, SEEK_SET);
    write(fileDesc_red1, red1_val, 1);

    char green1_val[2];
    sprintf(green1_val, "%d", arr[1]);
    lseek(fileDesc_green1, 0, SEEK_SET);
    write(fileDesc_green1, green1_val, 1);

    char blue1_val[2];
    sprintf(blue1_val, "%d", arr[2]);
    lseek(fileDesc_blue1, 0, SEEK_SET);
    write(fileDesc_blue1, blue1_val, 1);    

    return;
}

/**
 *  ledMatrix_setColourBottom
 *  Set the colour of the bottom part of the LED
 *  @params:
 *      int colour: colour to be set
 */
static void ledMatrix_setColourBottom(int colour)
{
    int arr[3] = {0,0,0};
    ledMatrix_bitsFromInt(arr, colour);

    // Write on the colour pins
    char red2_val[2];
    sprintf(red2_val, "%d", arr[0]);
    lseek(fileDesc_red2, 0, SEEK_SET);
    write(fileDesc_red2, red2_val, 1);

    char green2_val[2];
    sprintf(green2_val, "%d", arr[1]);
    lseek(fileDesc_green2, 0, SEEK_SET);
    write(fileDesc_green2, green2_val, 1);

    char blue2_val[2];
    sprintf(blue2_val, "%d", arr[2]);
    lseek(fileDesc_blue2, 0, SEEK_SET);
    write(fileDesc_blue2, blue2_val, 1);      

    return;
}

/**
 *  ledMatrix_refresh
 *  Fill the LED Matrix with the respective pixel colour
 */
static void ledMatrix_refresh(void)
{
    pthread_mutex_lock(&ledScreenMutex);
    {
        for ( int rowNum = 0; rowNum < 8; rowNum++ ) {
            lseek(fileDesc_oe, 0, SEEK_SET);
            write(fileDesc_oe, "1", 1); 
            ledMatrix_setRow(rowNum);
            for ( int colNum = 0; colNum < 32;  colNum++) {
                ledMatrix_setColourTop(screen[colNum][rowNum]);
                ledMatrix_setColourBottom(screen[colNum][rowNum+8]);
                ledMatrix_clock();
            }
            ledMatrix_latch();
            lseek(fileDesc_oe, 0, SEEK_SET);
            write(fileDesc_oe, "0", 1); 
            struct timespec reqDelay = {DELAY_IN_SEC, DELAY_IN_US}; // sleep for delay
            nanosleep(&reqDelay, (struct timespec *) NULL);
        }
    }
    pthread_mutex_unlock(&ledScreenMutex);
    // puts("Returning from refreshing LED.");
    return;
}

/**
 *  ledMatrix_setPixel
 *  Set the pixel selected on LED MAtrix with the colour selected
 *  @params:
 *      int x: x-axis
 *      int y: y-axis
 *      int colour: colour selected
 */
static void ledMatrix_setPixel(int x, int y, int colour)
{
    screen[y][x] = colour;

    return;
}

void LED_init(LED_Mode mode)
{
    pthread_mutex_init(&ledScreenMutex, NULL);
    sem_init(&ledSem, 0, 0);
    currentMode = mode;
    isBoardRunning = true;

    pthread_create(&ledThreadID, NULL, &ledThread, NULL);
}

void LED_cleanup(void)
{
    printf("Stopping the thread for displaying to the LED panel.\n");
    isBoardRunning = false;
    pthread_join(ledThreadID, NULL);
    pthread_mutex_destroy(&ledScreenMutex);

    sem_post(&ledSem);
    sleepForMs(100);
    sem_destroy(&ledSem);
}

void LED_wait(void)
{
    sem_wait(&ledSem);
}

void LED_setMode(int mode)
{
    if(mode > 2){
        mode = 0;
    }
    if(mode == -1){
        mode = 2;
    }
    currentMode = mode;
}

void LED_nextMode()
{
    // get the next mode
    currentMode++;
    LED_setMode(currentMode);
}

void LED_PreviousMode(){
    // get the previous mode
    currentMode--;
    LED_setMode(currentMode);
}

static int copy[32][16];
int (*getScreen())[16]{
    pthread_mutex_lock(&ledScreenMutex);
    // Create a new 2D array and copy the contents of the screen variable

    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 16; j++) {
            copy[i][j] = screen[i][j];
        }
    }
    // Unlock the mutex after accessing the screen variable
    pthread_mutex_unlock(&ledScreenMutex);
    // Lock the mutex before accessing the screen variable
    return copy;
}



LED_Mode LED_getMode()
{
    return currentMode;
}



void LED_setDisplay(const int row, const int col, const int matrix[row][col])
{
    if (currentMode == LED_LISTEN) {
        pthread_mutex_lock(&ledScreenMutex);
        {
            for (int rowNum = 0; rowNum < 16; rowNum++) {
                for (int colNum = 0; colNum < 32; colNum++) {
                    ledMatrix_setPixel(rowNum, colNum, matrix[rowNum][colNum]);
                }
            }
        }
        pthread_mutex_unlock(&ledScreenMutex);
    }
}

void LED_clearDisplay(void)
{
    pthread_mutex_lock(&ledScreenMutex);
    {
        memset(screen, 0, sizeof(screen));
    }
    pthread_mutex_unlock(&ledScreenMutex);
}

static void updateClockDisplay(void)
{
    /*
    *   Notes: 
    *   first number starts at screen[3][4]
    *   second number starts at screen[9][4]
    *   third number starts at screen[17][4]
    *   fourth number starts at screen[23][4]
    */

    int currentHour = CurrentTime_getCurrentHour();
    int currentMinute = CurrentTime_getCurrentMinute();

    int hourFirstDigit = currentHour / 10;
    int hourSecondDigit = currentHour % 10;
    int minuteFirstDigit = currentMinute / 10;
    int minuteSecondDigit = currentMinute % 10;

    // int **firstDigitMatrix = LEDMap_getNumberDisplay(hourFirstDigit);
    // int **secondDigitMatrix = LEDMap_getNumberDisplay(hourSecondDigit);
    // int **thirdDigitMatrix = LEDMap_getNumberDisplay(minuteFirstDigit);
    // int **fourthDigitMatrix = LEDMap_getNumberDisplay(minuteSecondDigit);
    int firstDigitMatrix[LED_TIME_ROW][LED_TIME_COL] = {0};
    int secondDigitMatrix[LED_TIME_ROW][LED_TIME_COL] = {0};
    int thirdDigitMatrix[LED_TIME_ROW][LED_TIME_COL] = {0};
    int fourthDigitMatrix[LED_TIME_ROW][LED_TIME_COL] = {0};

    int currentColor = Potentiometer_potToColor();

    LEDMap_getNumberDisplay(hourFirstDigit, currentColor, firstDigitMatrix);
    LEDMap_getNumberDisplay(hourSecondDigit, currentColor, secondDigitMatrix);
    LEDMap_getNumberDisplay(minuteFirstDigit, currentColor, thirdDigitMatrix);
    LEDMap_getNumberDisplay(minuteSecondDigit, currentColor, fourthDigitMatrix);

    const int colStartPositions[4] = {3, 9, 17, 23};

    LED_clearDisplay();

    pthread_mutex_lock(&ledScreenMutex);
    {
        // Display first digit
        for (int rowPos = 0; rowPos < NUMBER_SIZE_VERTICAL; rowPos++) {
            for (int colPos = 0; colPos < NUMBER_SIZE_HORIZONTAL; colPos++) {
                ledMatrix_setPixel(4 + rowPos, colStartPositions[0] + colPos, firstDigitMatrix[rowPos][colPos]);
            }
        }

        // Display second digit
        for (int rowPos = 0; rowPos < NUMBER_SIZE_VERTICAL; rowPos++) {
            for (int colPos = 0; colPos < NUMBER_SIZE_HORIZONTAL; colPos++) {
                ledMatrix_setPixel(4 + rowPos, colStartPositions[1] + colPos, secondDigitMatrix[rowPos][colPos]);
            }
        }

        // Display third digit
        for (int rowPos = 0; rowPos < NUMBER_SIZE_VERTICAL; rowPos++) {
            for (int colPos = 0; colPos < NUMBER_SIZE_HORIZONTAL; colPos++) {
                ledMatrix_setPixel(4 + rowPos, colStartPositions[2] + colPos, thirdDigitMatrix[rowPos][colPos]);
            }
        }

        // Display fourth digit
        for (int rowPos = 0; rowPos < NUMBER_SIZE_VERTICAL; rowPos++) {
            for (int colPos = 0; colPos < NUMBER_SIZE_HORIZONTAL; colPos++) {
                ledMatrix_setPixel(4 + rowPos, colStartPositions[3] + colPos, fourthDigitMatrix[rowPos][colPos]);
            }
        }

        // Display colon 
        ledMatrix_setPixel(6, 15, currentColor);
        ledMatrix_setPixel(8, 15, currentColor);
    }
    pthread_mutex_unlock(&ledScreenMutex);
}

static void* ledThread(void *vargp)
{
    struct sched_param sch_params;
    sch_params.sched_priority = SCHED_FIFO;
    int ret = pthread_setschedparam(pthread_self(), SCHED_FIFO, &sch_params);
    if (ret == EPERM) {
        perror("Permission error for elevating priority of LED driving thread");
    }
    else if (ret != 0) {
        perror("Error elevating priority of LED driving thread");
    }

    // int policy = 0;
    // if (pthread_getschedparam(pthread_self(), &policy, &sch_params) != 0) {
    //     puts("Error when checking priority level of LED thread.");
    // }
    // if (policy != SCHED_FIFO) {
    //     puts("Priority level of the thread is not actually SCHED_FIFO (1) even after changing.");
    //     printf("Actual priority level: %d\n", sch_params.sched_priority);
    // }

    // Reset the screen
    memset(screen, 0, sizeof(screen));

    // Setup pins
    ledMatrix_setupPins();

    printf("Thread for displaying to the LED panel now running.\n");
    while (isBoardRunning) {
        // Update the LED matrix
        if (currentMode == LED_OFF) {
            memset(screen, 0, sizeof(screen));
        }
        else if (currentMode == LED_CLOCK) {
            updateClockDisplay();
        }
        // Display the matrix
        ledMatrix_refresh();
        sleepForMs(5);
    }
    return 0;
}
