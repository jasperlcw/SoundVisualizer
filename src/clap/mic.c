#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include "circlebuffer.h"
#include "mic.h"


#define MIC "/sys/bus/iio/devices/iio:device0/in_voltage1_raw"
#define SHORT_TERM_SIZE 40
#define LONG_TERM_SIZE 734

static pthread_t reader;
static Circlebuffer* ShortTermBuffer;
static Circlebuffer* LongTermBuffer;
static bool stop = false;
static long long totalsamples = 0;
static int readFromFileToScreen(char *fileName){
    FILE *pFile = fopen(fileName, "r");
    if (pFile == NULL) {
        printf("ERROR: Unable to open file (%s) for read\n", fileName);
        exit(-1);
    }
    // Read string (line)
    const int MAX_LENGTH = 1024;
    char buff[MAX_LENGTH];
    fgets(buff, MAX_LENGTH, pFile);
    // Close
    fclose(pFile);
    int ret = atoi(buff);
    return ret;
  
    
}

static void sleepForMs(long long delayInMs)
{
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;
    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;
    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}



//creates the buffer and adds samples to it 
void* Sampler_startSamplingThread(){
    ShortTermBuffer = createBuffer(SHORT_TERM_SIZE);
    LongTermBuffer = createBuffer(LONG_TERM_SIZE);
    int val;
    //double voltage = 0;
    while(!stop){

        val = readFromFileToScreen(MIC);
        addBuffer(val, ShortTermBuffer);
        addBuffer(val, LongTermBuffer);
        sleepForMs(1);
        totalsamples++;
    }
    deleteBuffer(ShortTermBuffer);
    deleteBuffer(LongTermBuffer);    
    return NULL;
}

//starts the sampling thread 
void Mic_startSampling(){
    pthread_create(&reader,NULL, (void*)Sampler_startSamplingThread, NULL);
}

//returns a copy of the short tem buffer, user must free memory
double* Mic_getShortHistory(int *length){
    *length = getlength(ShortTermBuffer);
    double* i = copyBuffer(ShortTermBuffer);
    return i;
}
//returns a copy of the short tem buffer, user must free memory
double* Mic_getLongHistory(int *length){
    *length = getlength(LongTermBuffer);
    double* i = copyBuffer(LongTermBuffer);
    return i;
}

double* Mic_getLongHistoryWithIndex(int *length, int *index){
    double* ret = getBufferWithLen(length, index, LongTermBuffer);
    return ret;
}



long long Mic_getNumSamplesTaken(){
    return totalsamples; 
}

//stop sampling the mic
void Mic_stopSampling(){
    stop = true;
    pthread_join(reader,NULL);
    LongTermBuffer = NULL;
    ShortTermBuffer = NULL;
}
//clear the long term buffer
void Mic_Longclear(){
    clearBuffer(LongTermBuffer);
    while(getlength(LongTermBuffer) < 400);
}