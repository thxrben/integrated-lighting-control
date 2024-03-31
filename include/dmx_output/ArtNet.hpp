#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <thread>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <memory>

#include "Output.hpp"
#include "../dmx_engine/Universe.hpp"

#ifndef ARTNET_HPP
#define ARTNET_HPP

#define ARTNET_PORT 6454


class ArtNet : public Output {
    private:
        int socketFD;
        int opt;
        struct sockaddr_in address;   
        char* artnetHeader;
    public:
        ArtNet();
        void sendUniverse(std::shared_ptr<Universe> universe, unsigned char sequenceNum);
        void stop();
        void freeMemory();
        
};

#endif