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
#include "clap/clapdection.h"
#include "joystick/joystickcontrols.h"

#include <sys/resource.h>
#include <unistd.h>
#include <errno.h>

int main(void)
{
    // Priority_value = Nice_value + 20
    // Default niceness is 0

    // Setting priority occasionally causes audio buffer underflow 
    int ret = setpriority(PRIO_PROCESS, 0, -20);
    if (ret != 0) {
        perror("Error for setting priority");
        puts("This may cause LED panel to flicker at an increased amount.");
    }

    // Blocking call here until shutdown procedure initiated by UDP or joystick
    startMicDetection();
    joystickstart();

    LED_init(LED_LISTEN);
    AudioMixer_init();
    startBeatController();
    UDP_init();

    UDP_cleanup();
    clearBeatController();
    AudioMixer_cleanup();
    LED_wait();
    stopMicDectection();
}
