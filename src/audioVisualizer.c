#include "include/audioVisualizer.h"
#include "include/joystick.h"
#include "include/ledControl.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>

static pthread_t visualizerThreadID;
static sem_t runBlocker;
static bool isRunning;
static LED_Mode currentMode;

static void* visualizerThread(void *vargp);

void Visualizer_run(void)
{
    isRunning = true;
    sem_init(&runBlocker, 0, 0);
    pthread_create(&visualizerThreadID, NULL, visualizerThread, NULL);

    sem_wait(&runBlocker); // Block here until visualizer's shutdown process is complete
    sem_destroy(&runBlocker);
}

void Visualizer_shutdown(void)
{
    isRunning = false;
    pthread_join(visualizerThreadID, NULL);
    sem_post(&runBlocker);
}

// Private functions defined below
static void* visualizerThread(void *vargp)
{
    while (isRunning)
    {
        JS_Direction jsDirection = Joystick_getJoystickInput();

        switch (jsDirection)
        {
            case UP:
                currentMode = TIME;
                break;
            case DOWN:
                currentMode = OFF;
                break;
            case RIGHT:
                currentMode = RAINBOW;
                break;
            case LEFT:
                currentMode = VISUALIZER;
                break;
            case PRESS_IN:
                // shuts down the program
                break;
        }
    }
}