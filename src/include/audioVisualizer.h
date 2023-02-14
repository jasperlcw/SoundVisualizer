/* audioVisualizer.h
*  
*  This module provides the logic that runs the program for the BeagleBone app.
*/

#ifndef _VISUALIZER_H_
#define _VISUALIZER_H_

// Initializes the thread that runs the program itself.
void Visualizer_init(void);

// Cleans up the thread that runs the program itself.
void Visualizer_cleanup(void);

#endif