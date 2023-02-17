/* joystick.h
*  
*  This module provides an access point for a program to read the values of the joystick that is built in to the Zen Cape.
*/

#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

typedef enum JS_Direction
{
    UP,
    DOWN,
    RIGHT,
    LEFT,
    PRESS_IN,
    NONE
} JS_Direction;

// Initializes the joystick for use with the audio visualizer.
void Joystick_init(void);

// Reads the GPIO files for joystick control and returns the direction currently being pressed.
JS_Direction Joystick_getJoystickInput(void);

#endif