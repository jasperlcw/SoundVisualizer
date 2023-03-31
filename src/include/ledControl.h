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

// Initializes the I2C registers for displaying to the LED and starts
// with the mode being the passed in mode value.
void LED_init(LED_Mode mode);

// Cleans up the I2C registers for displaying to the LED.
void LED_cleanup(void);

// Halts a current thread until the module is stopped through LED_cleanup
void LED_wait(void);

// Sets the mode of the LED panel for what to display.
void LED_setMode(int mode);

// Sets next mode of the LED panel for what to display.
void LED_nextMode();
void LED_PreviousMode();

// Get the mode of the LED panel for what to display.
LED_Mode LED_getMode();

// Get the current Screen
int (*getScreen())[16];

// Based on a spectrum, sends a 16x32 matrix to the Adafruit board
void LED_projectSpectrum();

// Changes the colors displayed by the LED panel, where the passed in matrix
// should be a size of 16x32 containing values of the defined constants in this module.
// Note that the program will ignore inputs if the mode is set to CLOCK.
void LED_setDisplay(const int row, const int col, const int matrix[row][col]);

// Clears the LED matrix (makes it all show black)
void LED_clearDisplay(void);

#endif