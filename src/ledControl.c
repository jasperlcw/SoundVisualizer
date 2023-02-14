#include "include/ledControl.h"

static LED_Mode currentMode;

// Function prototypes for displaying the current mode.
static void displayOff(void);
static void displayRainbow(void);
static void displayVisualizer(void);
static void displayTime(void);
