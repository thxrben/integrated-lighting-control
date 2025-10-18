#include "dmx_output/ArtNet.hpp"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"

#define ANSI_COLOR_RESET   "\x1b[0m"

ArtNet::ArtNet() {

    artnetHeader = (char *) malloc(19);

    *(artnetHeader + 0) = 'A';
    *(artnetHeader + 1) = 'r';
    *(artnetHeader + 2) = 't';
    *(artnetHeader + 3) = '-';
    *(artnetHeader + 4) = 'N';
    *(artnetHeader + 5) = 'e';
    *(artnetHeader + 6) = 't';
    *(artnetHeader + 7) = '\0';
    
    // Little Endian: Artnet-Opcode:
    *(artnetHeader + 8) = 0x00;
    *(artnetHeader + 9) = 0x50;

    // Prot Version Hi - Low
    *(artnetHeader + 10) = 0x00;
    *(artnetHeader + 11) = 0x0e; // Version 14


    if ((socketFD = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Could not create UDP-Socket for ArtNet Output!");
        exit(1);
    }

    int broadcastEnable = 1;

    if(setsockopt(socketFD, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) < 0) {
        printf("[ERROR] Artnet Socket Options failed!\n");
        exit(1);
    }

    /* set up destination address */
    memset(&address,0,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("10.10.40.102");
    address.sin_port=htons(ARTNET_PORT);

    printf(ANSI_COLOR_RESET "[" ANSI_COLOR_CYAN "ArtNet" ANSI_COLOR_RESET "] : ArtNet-Output started successfully.\n");
};  


void ArtNet::sendUniverse(std::shared_ptr<Universe> universe, unsigned char sequenceNum) {

    char* finalArray = (char*) malloc(18 + 512); // 19 is Headersize + 512 Data
    
    unsigned short universeNum = universe->getUniverseNum();

    *(this->artnetHeader + 12) = sequenceNum;
    *(this->artnetHeader + 13) = universeNum & 0xFF;
    *(this->artnetHeader + 14) = universeNum & 0xFF; // Universe LOW
    *(this->artnetHeader + 15) = (universeNum >> 8) & 0xFF; // Universe HIGH

    *(this->artnetHeader + 16) = 0x02; // Length (Num of DMX Channels) HIGH
    *(this->artnetHeader + 17) = 0x00; // LOW

    memcpy(finalArray, this->artnetHeader, 19);

    memcpy((finalArray + 18), universe->getDMXValues(), 512); // Copy the DMX-Values into the network packet

    int res = sendto(socketFD, finalArray, 512+18, 0,(struct sockaddr *) &address, sizeof(this->address));

    if (res < 0)
    {
        fprintf(stderr, "RES: %d\n", res);
        perror("sendto");
        return;
    }
    if(finalArray == nullptr)
        return;
    free(finalArray);
    finalArray = nullptr;
};

void ArtNet::freeMemory() {
    if(this->artnetHeader == nullptr)
        return;
    free(this->artnetHeader);
    this->artnetHeader = nullptr;
}

void ArtNet::stop() {
    close(socketFD);
}
