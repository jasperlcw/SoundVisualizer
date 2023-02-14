#include "include/joystick.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define JS_UP_DIRECTION_PATH "/sys/class/gpio/gpio26/direction"
#define JS_DOWN_DIRECTION_PATH "/sys/class/gpio/gpio46/direction"
#define JS_RIGHT_DIRECTION_PATH "/sys/class/gpio/gpio47/direction"
#define JS_LEFT_DIRECTION_PATH "/sys/class/gpio/gpio65/direction"
#define JS_UP_VALUE_PATH "/sys/class/gpio/gpio26/value"
#define JS_DOWN_VALUE_PATH "/sys/class/gpio/gpio46/value"
#define JS_RIGHT_VALUE_PATH "/sys/class/gpio/gpio47/value"
#define JS_LEFT_VALUE_PATH "/sys/class/gpio/gpio65/value"

void Joystick_init(void)
{
    FILE *jsUpDirection = fopen(JS_UP_DIRECTION_PATH, "w");
    FILE *jsDownDirection = fopen(JS_DOWN_DIRECTION_PATH, "w");
    FILE *jsLeftDirection = fopen(JS_LEFT_DIRECTION_PATH, "w");
    FILE *jsRightDirection = fopen(JS_RIGHT_DIRECTION_PATH, "w");

    // TODO: fprintf "in" to the gpio direction files
}

JS_Direction Joystick_getJoystickInput(void)
{
    FILE *jsUpValue = fopen(JS_UP_VALUE_PATH, "w");
    FILE *jsDownValue = fopen(JS_DOWN_VALUE_PATH, "w");
    FILE *jsRightValue = fopen(JS_RIGHT_VALUE_PATH, "w");
    FILE *jsLeftValue = fopen(JS_LEFT_VALUE_PATH, "w");

    // TODO: add error check here
    
    // TODO: read the files and return a JS_Direction as specified from the gpio value files
}