//circle buffer interface, allows size to be changed 
#ifndef circlebuffer_h
#define circlebuffer_h

typedef struct{
    double* buffer;
    int size;
    int index;
    int numobjects;
}Circlebuffer;
//creates a buffer size of n
Circlebuffer* createBuffer(int n);

//adds elements to the buffer
void addBuffer(double val, Circlebuffer* buf);

///clears the values of the buffer
void clearBuffer(Circlebuffer* buf);

//returns copy of the buffer reciever must free
double* copyBuffer(Circlebuffer* buf);

//returns the length of the buffer
int getlength(Circlebuffer* buf);

//deletes the buffer and frees memory associated with the buffer 
void deleteBuffer(Circlebuffer* buf);

#endif