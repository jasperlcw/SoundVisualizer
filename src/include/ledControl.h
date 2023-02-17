/* ledControl.h
*  
*  Module for outputting LED visual effects to the Adafruit 16x32 LED panel.
*/

#ifndef _LED_H_
#define _LED_H_

// Initializes the I2C registers for displaying to the LED.
void LED_init(void);

// Cleans up the I2C registers for displaying to the LED.
void LED_cleanup(void);

// TODO: figure out a way to give this module data that it can easily convert to display
// on an LED panel. Maybe a 2D matrix of hex values?

// something along the lines of 
// void LED_display(2D matrix of hex);

#endif