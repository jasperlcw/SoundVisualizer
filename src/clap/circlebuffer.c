#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "circlebuffer.h"
#include <pthread.h>
#include <string.h>

static pthread_mutex_t lock;

//creates a buffer size of n
Circlebuffer* createBuffer(int n){
    pthread_mutex_init(&lock, NULL);
    pthread_mutex_lock(&lock);
    Circlebuffer* ret = malloc(sizeof(*ret));
    if(!ret){
        exit(EXIT_FAILURE);
    }
    ret->buffer = malloc(sizeof(*ret->buffer)* n);
    if(!ret->buffer){
        exit(EXIT_FAILURE);
    }
    ret->index = -1;
    ret->size = n;
    ret->numobjects = 0;
    pthread_mutex_unlock(&lock);

    return ret;
}

//returns copy of the buffer reciever must free
double* copyBuffer(Circlebuffer* buf){
    double* newbuf = malloc(sizeof(*newbuf) * buf->size);
    while(buf->numobjects < buf->size);
    pthread_mutex_lock(&lock);
    memcpy(newbuf, buf->buffer, (buf->numobjects*sizeof(double)));
    pthread_mutex_unlock(&lock);
    return newbuf;
}
//adds elements to the buffer
void addBuffer(double val, Circlebuffer* buf){
    pthread_mutex_lock(&lock);
    if(((buf->index + 1) == (buf->size))){
        buf->index = -1;
    }
    buf->index++;
    buf->buffer[buf->index] = val;
    buf->numobjects++;
    if(buf->numobjects > buf->size){
        buf->numobjects = buf->size;
    }
    pthread_mutex_unlock(&lock);

}
//returns the length of the buffer
int getlength(Circlebuffer* buf){
    return buf->numobjects;
}
///clears the values of the buffer
void clearBuffer(Circlebuffer* buf){
    pthread_mutex_lock(&lock);
    buf->index = 0;
    buf->numobjects = 0;
    pthread_mutex_unlock(&lock);
}

double* getBufferWithLen(int *length, int *index, Circlebuffer* buf){

    double* newbuf = malloc(sizeof(*newbuf) * buf->size);
    while(buf->numobjects < buf->size);
    pthread_mutex_lock(&lock);
    *length = buf->size;
    *index = buf->index;
    memcpy(newbuf, buf->buffer, (buf->numobjects*sizeof(double)));
    pthread_mutex_unlock(&lock);
    return newbuf;
}


//deletes the buffer and frees memory associated with the buffer 
void deleteBuffer(Circlebuffer* buf){
    pthread_mutex_lock(&lock);
    free(buf->buffer);
    buf->buffer = NULL;
    free(buf);
    buf = NULL;
    pthread_mutex_unlock(&lock);
}