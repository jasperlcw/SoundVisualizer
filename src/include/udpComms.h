/* udpComms.h
*
*  Module for the networking component of the audio visualizer program.
*/

#ifndef _UDP_H_
#define _UDP_H_

#include "../Utility.h"


// Initializes the UDP thread.
void UDP_init(void);

// Cleans up the UDP thread.
void UDP_cleanup(void);

// Initializes the UDP server
void* StartUDPServer();
void* emitMessage();

#endif