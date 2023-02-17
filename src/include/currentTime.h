/* currentTime.h
*
*  Module for getting the current system time.
*/

#ifndef _CURRENT_TIME_H_
#define _CURRENT_TIME_H_

#include <time.h>

// Get the current system time.
time_t CurrentTime_getCurrentTime(void);

// Get the hour of current system time.
int CurrentTime_getCurrentHour(void);

// Get the minute of the current system time.
int CurrentTime_getCurrentMinute(void);

// Get the second of the current system time.
int CurrentTime_getCurrentSecond(void);

#endif