/* main.c
*  
*  Entry point into the program. Calls the threads and waits until the threads that run the program all end.
*  Logic of the program should be implemented in audioVisualizer.c
*/

//#include "include/audioVisualizer.h"
#include "include/udpComms.h"
#include "audioMixer/audioMixer_template.h"
#include "audioMixer/BeatController.h"
#include "include/ledControl.h"

int main(void)
{
    // Blocking call here until shutdown procedure initiated by UDP or joystick

    LED_init(LED_LISTEN);
    AudioMixer_init();
    startBeatController();
    UDP_init();
    UDP_cleanup();
    clearBeatController();
    AudioMixer_cleanup();
    LED_wait();
//    Visualizer_run();
}