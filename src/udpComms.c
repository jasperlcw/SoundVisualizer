#include "audioMixer/audioMixer_template.h"
#include "audioMixer/BeatController.h"
#include "include/ledControl.h"
#include "include/potentiometer.h"
#include "include/udpComms.h"

// modules required
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <netdb.h>
#include <unistd.h>			// for close()

#include <semaphore.h>
#include <math.h>
#include <pthread.h>

// const
#define MSG_MAX_LEN 1024
#define PORT        12345

static pthread_t UDPThreadID;
static sem_t UDPRunBlocker;

// previous command
char previousCmd[MSG_MAX_LEN];

static void displayMatrixOnLed(int row, int col, double *matrix);

// Initializes the UDP thread.
void UDP_init(void) {
    sem_init(&UDPRunBlocker, 0, 0);
    pthread_create(&UDPThreadID, NULL, StartUDPServer, NULL);

    sem_wait(&UDPRunBlocker); // Block here until visualizer's shutdown process is complete
    sem_destroy(&UDPRunBlocker);
}

// Cleans up the UDP thread.
void UDP_cleanup(void) {
    pthread_join(UDPThreadID, NULL);
    sem_post(&UDPRunBlocker);
}

static char tempJson[1000];

char* doubleArrayToJson(double* array, int size, char* key){

    memset(tempJson, 0, sizeof(char) * 1000);

    char* stringOffset = tempJson;
    stringOffset+= sprintf(stringOffset, "{\"%s\":[", key);

    for(int i = 0; i < size-1; i++){
        stringOffset += sprintf(stringOffset, "%f,", array[i]);
    }

    stringOffset += sprintf(stringOffset, "%f", array[size-1]);

    stringOffset += sprintf(stringOffset, "]}");

    return tempJson;
}

char* doubleArrayToJsonWithColor(double* array, int size, char* key, LED_Mode mode){

    memset(tempJson, 0, sizeof(char) * 1000);

    char* stringOffset = tempJson;
    stringOffset+= sprintf(stringOffset, "{\"data\":");

    stringOffset+= sprintf(stringOffset, "{\"%s\":[", key);

    for(int i = 0; i < size-1; i++){
        stringOffset += sprintf(stringOffset, "%0.3f,", array[i]);
    }

    stringOffset += sprintf(stringOffset, "%0.3f", array[size-1]);

    stringOffset += sprintf(stringOffset, "],\"%s\":%d}", "color", mode);

    stringOffset+= sprintf(stringOffset, "}");

    return tempJson;
}

char* Int2DArrayToJson(int (*array)[16], int rows, int cols, char* key){

    memset(tempJson, 0, sizeof(char) * 1024);

    char* stringOffset = tempJson;
    stringOffset+= sprintf(stringOffset, "{\"%s\":[", key);

    for(int i = 0; i < rows; i++){
        stringOffset += sprintf(stringOffset, "[");

        for(int j = 0; j < cols-1; j++){
            stringOffset += sprintf(stringOffset, "%d,", array[i][j]);
        }

        stringOffset += sprintf(stringOffset, "%d", array[i][cols-1]);

        if(i == rows-1){
            stringOffset += sprintf(stringOffset, "]");
        } else {
            stringOffset += sprintf(stringOffset, "],");
        }
    }

    stringOffset += sprintf(stringOffset, "]}");

    return tempJson;
}



void* StartUDPServer(){
    // Address
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;                   // Connection may be from network
    sin.sin_addr.s_addr = htonl(INADDR_ANY);    // Host to Network long
    sin.sin_port = htons(PORT);                 // Host to Network short

    // Create the socket for UDP
    int socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);

    // Bind the socket to the port (PORT) that we specify
    bind (socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));

    //error check
    if(!socketDescriptor){
        printf("UDP Server is unable to start\n");
        return NULL;
    }
    printf("UDP Server is ready!\n");

    while(isRunning){

        // Get the data (blocking)
        // Will change sin (the address) to be the address of the client.
        // Note: sin passes information in and out of call!
        struct sockaddr_in sinRemote;
        unsigned int sin_len = sizeof(sinRemote);
        char messageRx[MSG_MAX_LEN];

        // Pass buffer size - 1 for max # bytes so room for the null (string data)
        int bytesRx = recvfrom(socketDescriptor,
                               messageRx, MSG_MAX_LEN - 1, 0,
                               (struct sockaddr *) &sinRemote, &sin_len);

        // Check for errors (-1)

        // Make it null terminated (so string functions work)
        // - recvfrom given max size - 1, so there is always room for the null
        messageRx[bytesRx] = 0;
//         printf("Message received (%d bytes): %s\n", bytesRx, messageRx);

        // separate the command and store into an array
        char *temp;
        char *cmd[MSG_MAX_LEN];

        temp = strtok(messageRx," ");
        int count = 0;
        while (temp != NULL){
            cmd[count] = temp;
            temp = strtok(NULL," \n");
            count ++;
        }

        //message to transfer;
        char messageTx[MSG_MAX_LEN];

        // empty cmd received, store previous cmd into current cmd
        if(strcmp(cmd[0], "\n") == 0){
            sprintf(*cmd, "%s",previousCmd);
        }
        else{
            // store current cmd to previous cmd
            sprintf(previousCmd, "%s", *cmd);
        }
        // help
        if(strcmp(cmd[0], "help\n") == 0){
            sprintf(previousCmd, "help\n");
            sprintf(messageTx, "Accepted command examples:\n"
                               "timeFormat 0 -- (0) AM/PM clock (1) 24h clock.\n");
        }
        //time
        else if(strcmp(cmd[0], "timeFormat\n") == 0){
            int choice = atoi(cmd[1]);
            if(choice == 0){

                //todo: set time format here

                sprintf(messageTx, "Time format has set to AM/PM clock:\n");
            }
            else if(choice == 1){
                //todo: set time format here

                sprintf(messageTx, "Time format has set to 24h clock:\n");
            }
            else{
                sprintf(messageTx, "invalid choice for time format, only 0 or 1 is available:\n");
            }
        }
        //get Spectrum
        else if(strcmp(cmd[0], "getSpectrum") == 0){
            double * spectrum = getSpectrum();
            int spectrumSize = getSpectrumCount();
            int color = Potentiometer_potToColor();

            displayMatrixOnLed(16, 32, spectrum);
            doubleArrayToJsonWithColor(spectrum, spectrumSize, "value", color);
            sprintf(messageTx, "spectrum %s", tempJson);
        }
        //get Brightness
        else if(strcmp(cmd[0], "getBrightness") == 0){
            int brightnessPercent = Potentiometer_getReading();
            sprintf(messageTx, "brightness %d", brightnessPercent);
        }
        // set new Music
        else if(strcmp(cmd[0], "setMusic") == 0){
            sprintf(messageTx, "200");
            initialUploadWave();
        }

        //get current running mode
        else if(strcmp(cmd[0], "getMode") == 0){
            sprintf(messageTx, "mode %d", LED_getMode());
        }

        //set current running mode
        else if(strcmp(cmd[0], "setMode") == 0){
            LED_setMode(atoi(cmd[1]));
            LED_clearDisplay();
            sprintf(messageTx, "200");
        }
        //set +1 to current running mode
        else if(strcmp(cmd[0], "setNextMode") == 0){
            LED_nextMode();
            LED_clearDisplay();
            sprintf(messageTx, "200");
        }
        //set -1 to current running mode
        else if(strcmp(cmd[0], "setPreviousMode") == 0){
            LED_PreviousMode();
            LED_clearDisplay();
            sprintf(messageTx, "200");
        }
        // set volume
        else if(strcmp(cmd[0], "setVolume") == 0){
            int volume = AudioMixer_setVolume(atoi(cmd[1]));
            sprintf(messageTx, "volume %d", volume);
        }
        // get volume
        else if(strcmp(cmd[0], "getVolume") == 0){
            int volume = AudioMixer_getVolume();
            sprintf(messageTx, "volume %d", volume);
        }

        //press the screen
        else if(strcmp(cmd[0], "getScreen") == 0){
            int (*screen)[16] = getScreen();
            Int2DArrayToJson(screen, 32, 16, "value");
            sprintf(messageTx, "screen %s", tempJson);
        }
        
        else{
            sprintf(messageTx, "Unknown command. Type 'help' for command list.\n");
            // Transmit a reply:
        }
//        printf("%s", cmd[0]);
        sin_len = sizeof(sinRemote);
        sendto( socketDescriptor,
                messageTx, strlen(messageTx),
                0,
                (struct sockaddr *) &sinRemote, sin_len);
    }
    // Close
    close(socketDescriptor);

    return 0;
}

// Helper function for displaying generated spectrum to the LED Panel
static void displayMatrixOnLed(int row, int col, double *matrix)
{
    if (!matrix) {
        return;
    }

    int ledMatrix[16][32] = {0};
    for (int colNum = 0; colNum < 32; colNum++) {
        int logValue = log10(matrix[colNum]) > 1 ? log10(matrix[colNum]) : 1;
        int barHeight = floor(matrix[colNum] / 10 * (16 / logValue));

        int currentIndex = 0;
        for (int rowNum = 15; rowNum >= 0; rowNum--) {
            if (currentIndex < barHeight) {
                ledMatrix[rowNum][colNum] = Potentiometer_potToColor();
            }
            currentIndex++;
        }
        // LED_setDisplay(row, col, ledMatrix);
    }
    LED_setDisplay(row, col, ledMatrix);
}
