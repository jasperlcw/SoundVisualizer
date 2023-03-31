//starts the clap dection and samples values from the mic
//isClap return true a double clap is detected 
//https://stackoverflow.com/questions/17167949/how-to-use-timer-in-c used to help create timer
#ifndef clapdection_h
#define clapdection_h
#include <stdbool.h> 
//starts the clap detection also creates a thread that samples that record mic values 
void startMicDetection();
//stop the clap dection thread and stops sampling values from the mic
void stopMicDectection();
//returns true if double clap is detected
bool isClap();
void clapOn(bool val);
#endif
