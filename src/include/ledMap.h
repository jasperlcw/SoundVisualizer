/*  ledMap.h
*
*   Module to provide utility for converting numbers to a matrix
*   representation for the Adafruit LED board.
*/

#ifndef _LEDMAP_H_
#define _LEDMAP_H_

// Returns a dynamically allocated array to a LED representation of the passed in number.
// If an invalid number is passed in, it will return an array with all values being zero.
int** LEDMap_getNumberDisplay(const int number);

// Frees the dynamically allocated array returned by LED_getNumberDisplay.
void LEDMap_freeDisplayArray(int **toFree);

#endif
