// potentiometer.h
// A module that reads the potentiometer value for other modules
// to consume.

#ifndef _POTENTIOMETER_H_
#define _POTENTIOMETER_H_ 

// Returns a value from 0 - 100 (inclusive) representing the percentage
// of the way the potentiometer has been turned
int Potentiometer_getReading(void);

#endif
