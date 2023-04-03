/*  ledMap.h
*
*   Module to provide utility for converting numbers to a matrix
*   representation for the Adafruit LED board.
*/

#ifndef _LEDMAP_H_
#define _LEDMAP_H_

#define LED_TIME_ROW 7
#define LED_TIME_COL 5

// Returns a dynamically allocated array to a LED representation of the passed in number.
// If an invalid number is passed in, it will return an array with all values being zero.
void LEDMap_getNumberDisplay(const int number, int timeMatrix[LED_TIME_ROW][LED_TIME_COL]);

#endif
