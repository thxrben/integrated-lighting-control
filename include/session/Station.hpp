#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifndef STATION_HPP
#define STATION_HPP


class Station {
    public:
        Station();
        Station(in_addr ipAddr);
    private:
        in_addr ipAddr;
        uint64_t macAddr;
};

#endif