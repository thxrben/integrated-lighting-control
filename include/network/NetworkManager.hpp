#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>	
#include <vector>

#include "../config/Config.hpp"

#ifndef NETWORK_MANAGER_HPP
#define NETWORK_MANAGER_HPP

typedef struct  {
    in_addr ipAddr;
    in_addr netmask;
    in_addr broadcast;
} interfaceMeta;

class NetworkManager {

    public:
        NetworkManager();
        ~NetworkManager();
        void updateIPs();
        int set_interface_address(const char* interface, const char* address, const char* netmask);
        std::vector<interfaceMeta> retrieveInterfaces();
    private:
        std::vector<interfaceMeta> interfaces;
};

#endif