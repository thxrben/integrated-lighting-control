#include "SocketProvider.hpp"

void SocketProvider::internalOpen(interfaceMeta interface, boolean broadcastEnable, int port) {

    /* set up local address and port of the socket*/
    memset(&localAddress, 0, sizeof(localAddress));
    localAddress.sin_family = AF_INET;
    localAddress.sin_port = htons(port);
    localAddress.sin_addr = interface.ipAddr;

    /* set up broadcast address */
    memset(&broadcastAddress, 0, sizeof(broadcastAddress));
    broadcastAddress.sin_family = AF_INET;
    broadcastAddress.sin_port = htons(port);
    broadcastAddress.sin_addr = interface.broadcast;


    //Datagram (UDP) Socket
    this->socketFD = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Enable non-blocking recvfrom:
    socketFlags = fcntl(this->socketFD, F_GETFL);
    socketFlags |= O_NONBLOCK;
    fcntl(this->socketFD, F_SETFL, socketFlags);


    if (this->socketFD < 0)
    {
        perror("[SessionAPI] Could not create UDP-Socket for SessionAPI Output!");
        exit(1);
    }

    // If Broadcast is desired, set the necessary socket options 
    if(broadcastEnable) {
        int broadcast = 1;
        
        if (setsockopt(this->socketFD, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0) {
            printf("[SessionAPI] Socket Options failed - Could not enable broadcast for port %d!\n", port);
            exit(2);
        }


        if (bind(this->socketFD, (struct sockaddr *) &broadcastAddress, sizeof(broadcastAddress)) < 0) {
            perror("[SessionAPI] Error binding socket");
            exit(5);
        }
    } else {

        if (bind(this->socketFD, (struct sockaddr *) &localAddress, sizeof(localAddress)) < 0) {
            perror("[SessionAPI] Error binding socket");
            exit(5);
        }

    }




};