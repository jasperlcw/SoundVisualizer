/* main.c
*  
*  Entry point into the program. Calls the threads and waits until the threads that run the program all end.
*  Logic of the program should be implemented in audioVisualizer.c
*/
#include "include/audioVisualizer.h"

int main(void)
{
    // Blocking call here until shutdown procedure initiated by UDP or joystick
    Visualizer_run();
}