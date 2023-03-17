/* ledControl.h
*  
*  Module for outputting LED visual effects to the Adafruit 16x32 LED panel.
*/

#ifndef _LED_H_
#define _LED_H_

#define LED_BLACK   0
#define LED_RED     1
#define LED_GREEN   2
#define LED_YELLOW  3
#define LED_BLUE    4
#define LED_MAGENTA 5
#define LED_TEAL    6
#define LED_WHITE   7

typedef enum LED_Mode {
    LED_OFF,
    LED_CLOCK,
    LED_LISTEN
} LED_Mode;

// Matrtices are declared as [row][column]
// Each number takes up 7 rows and 5 columns
int LED_numZero[7][5] = {
    { 0, 1, 1, 1, 0 },
    { 1, 0, 0, 0, 1 },
    { 1, 0, 0, 1, 1 },
    { 1, 0, 1, 0, 1 },
    { 1, 1, 0, 0, 1 },
    { 1, 0, 0, 0, 1 },
    { 0, 1, 1, 1, 0 }
};

int LED_numOne[7][5] = {
    { 0, 0, 1, 0, 0 },
    { 0, 1, 1, 0, 0 },
    { 0, 0, 1, 0, 0 },
    { 0, 0, 1, 0, 0 },
    { 0, 0, 1, 0, 0 },
    { 0, 0, 1, 0, 0 },
    { 0, 1, 1, 1, 0 }
};

int LED_numTwo[7][5] = {
    { 0, 1, 1, 1, 0 },
    { 1, 0, 0, 0, 1 },
    { 0, 0, 0, 0, 1 },
    { 0, 1, 1, 1, 0 },
    { 1, 0, 0, 0, 0 },
    { 1, 0, 0, 0, 0 },
    { 1, 1, 1, 1, 1 }
};

int LED_numThree[7][5] = {
    { 1, 1, 1, 1, 0 },
    { 0, 0, 0, 0, 1 },
    { 0, 0, 0, 0, 1 },
    { 0, 1, 1, 1, 0 },
    { 0, 0, 0, 0, 1 },
    { 0, 0, 0, 0, 1 },
    { 1, 1, 1, 1, 0 }
};

int LED_numFour[7][5] = {
    { 0, 0, 1, 0, 1 },
    { 0, 1, 0, 0, 1 },
    { 1, 0, 0, 0, 1 },
    { 1, 1, 1, 1, 1 },
    { 0, 0, 0, 0, 1 },
    { 0, 0, 0, 0, 1 },
    { 0, 0, 0, 0, 1 }
};

int LED_numFive[7][5] = {
    { 1, 1, 1, 1, 1 },
    { 1, 0, 0, 0, 0 },
    { 1, 1, 1, 1, 0 },
    { 0, 0, 0, 0, 1 },
    { 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1 },
    { 0, 1, 1, 1, 0 }
};

// TODO: implement
int LED_numSix[7][5] = {
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 }
};

int LED_numSeven[7][5] = {
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 }
};

int LED_numEight[7][5] = {
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 }
};

int LED_numNine[7][5] = {
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 }
};

// Initializes the I2C registers for displaying to the LED.
void LED_init(void);

// Cleans up the I2C registers for displaying to the LED.
void LED_cleanup(void);

// Sets the mode of the LED panel for what to display.
void LED_setMode(LED_Mode mode);

// Changes the colors displayed by the LED panel, where the passed in matrix
// should be a size of 16x32 containing values of the defined constants in this module.
// Note that the program will ignore inputs if the mode is set to CLOCK.
void LED_setDisplay(int **matrix);

#endif