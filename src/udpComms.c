#include "include/udpComms.h"


static pthread_t UDPThreadID;
static sem_t UDPRunBlocker;

// previous command
char previousCmd[MSG_MAX_LEN];


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
        printf("Message received (%d bytes): %s\n", bytesRx, messageRx);

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
            sprintf(cmd[0], "%s",previousCmd);
        }
        else{
            // store current cmd to previous cmd
            sprintf(previousCmd, "%s\n", cmd[0]);
        }

        // help
        if(strcmp(cmd[0], "help\n") == 0){
            sprintf(previousCmd, "help\n");
            sprintf(messageTx, "Accepted command examples:\n"
                               "timeFormat 0 -- (0) AM/PM clock (1) 24h clock.\n");

            sin_len = sizeof(sinRemote);
            sendto( socketDescriptor,
                    messageTx, strlen(messageTx),
                    0,
                    (struct sockaddr *) &sinRemote, sin_len);
        }
        //time
        else if(strcmp(cmd[0], "timeFormat") == 0){
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
            // Transmit a reply:
            sin_len = sizeof(sinRemote);
            sendto( socketDescriptor,
                    messageTx, strlen(messageTx),
                    0,
                    (struct sockaddr *) &sinRemote, sin_len);
        }
        //
        else{
            sprintf(messageTx, "Unknown command. Type 'help' for command list.\n");
            // Transmit a reply:
            sin_len = sizeof(sinRemote);
            sendto( socketDescriptor,
                    messageTx, strlen(messageTx),
                    0,
                    (struct sockaddr *) &sinRemote, sin_len);
        }
    }
    // Close
    close(socketDescriptor);

    return 0;
}

