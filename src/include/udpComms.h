/* udpComms.h
*
*  Module for the networking component of the audio visualizer program.
*/

#ifndef _UDP_H_
#define _UDP_H_

// Initializes the UDP thread.
void UDP_init(void);

// Cleans up the UDP thread.
void UDP_cleanup(void);

#endif