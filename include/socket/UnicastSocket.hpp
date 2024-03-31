#ifndef UNICAST_SOCKET_HPP
#define UNICAST_SOCKET_HPP

#include "SocketProvider.hpp"

class UnicastSocket : SocketProvider {
    public:
        virtual void open(int port);
    private:
};

#endif