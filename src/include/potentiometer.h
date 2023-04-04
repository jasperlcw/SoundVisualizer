// potentiometer.h
// A module that reads the potentiometer value for other modules
// to consume.

#ifndef _POTENTIOMETER_H_
#define _POTENTIOMETER_H_ 

// Returns a value from 0 - 100 (inclusive) representing the percentage
// of the way the potentiometer has been turned
int Potentiometer_getReading(void);

// Returns a color (defined in ledControl module) based on the POT reading.
int Potentiometer_potToColor(void);

int getVoltageReading(char* path);
void Potstart(void);
void Potcleanup(void);
int getPotValue(void);

#endif
