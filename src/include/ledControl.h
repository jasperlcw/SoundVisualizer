/* ledControl.h
*  
*  Module for outputting LED visual effects to the Adafruit 16x32 LED panel.
*/

#ifndef _LED_H_
#define _LED_H_

typedef enum LED_Mode
{
    OFF,
    RAINBOW,
    VISUALIZER,
    TIME,
    NUM_MODES
} LED_Mode;

// Initializes the thread for displaying to the LED.
void LED_init(void);

// Cleans up the thread for displaying to the LED.
void LED_cleanup(void);

// Changes the display mode of the LED based on what is passed in to it.
void LED_changeMode(LED_Mode mode);

// Queries the LED module for the current mode that it is displaying.
void LED_currentMode(void);

#endif