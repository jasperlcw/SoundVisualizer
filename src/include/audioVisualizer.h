/* audioVisualizer.h
*  
*  This module provides the logic that runs the program for the BeagleBone app.
*/

#ifndef _VISUALIZER_H_
#define _VISUALIZER_H_

typedef enum LED_Mode
{
    OFF,
    RAINBOW,
    VISUALIZER,
    TIME,
    NUM_MODES
} LED_Mode;

// Initializes the thread that runs the program itself.
// This function blocks until shutdown is called.
void Visualizer_run(void);

// Shuts down and cleans up the thread that runs the program itself.
void Visualizer_shutdown(void);

#endif