#ifndef SOCKETPROVIDER_HPP
#define SOCKETPROVIDER_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "../network/NetworkManager.hpp"

class SocketProvider {
    public:
        virtual void open(int port);
        virtual void close();
    
    protected:
        int port;
        int socketFD;
        bool isClosed;
        int socketFlags;
        void internalOpen(interfaceMeta interface, int socketType, int port);
        struct sockaddr_in localAddress;   
        struct sockaddr_in broadcastAddress;

};

#endif