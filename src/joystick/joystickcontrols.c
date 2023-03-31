#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include "joystick.h"
#include "../audioMixer/audioMixer_template.h"
#include "../Utility.h"
#define defualtBPM 80
bool run;
static pthread_t joystickthread;


static void* joystick(){
    
    while(run){
        for(int i = 0; i < JOYSTICK_MAX_NUMBER_DIRECTIONS; i++){
            
            if(Joystick_isDirectionPressed(i)){
                switch (i)
                {
                case 0://up case
             
                     //sleepForMs(100);

                    break;
                case 1: //down case
                  
                     //sleepForMs(100);
                    break;
                case 2: //left case
       
                     //sleepForMs(100);
                     break;
                case 3: //right case
             
                    // sleepForMs(100);
                     break;
                case 4: //pressed
                    changeAudioIn();





                    sleepForMs(100);
                     break;
                default:
                    break;
                }
            }
        }
        sleepForMs(100);
    }
    return NULL;

}
//start the joystick thread
void joystickstart(){
    run = true;
    Joystick_init();
    pthread_create(&joystickthread, NULL, joystick, NULL);
}
//ends the joystick thread
void joystickcleanup(){
    run = false;
    pthread_join(joystickthread,NULL);
   
}

