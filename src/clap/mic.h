//reads the mic values into two buffer one short term and a long term bufffer

#ifndef mic_h
#define mic_h
//creates the buffer and adds samples to it 
void Mic_startSampling();

//stop sampling the mic
void Mic_stopSampling();

//returns the history of the short term buffer
double* Mic_getShortHistory(int *length);

//returns the history of the long term buffer
double* Mic_getLongHistory(int *length);

double* Mic_getLongHistoryWithIndex(int *length, int *index);

//returns the total number of samples taken
long long Mic_getNumSamplesTaken();

//clears the long term buffer
void Mic_Longclear();
#endif