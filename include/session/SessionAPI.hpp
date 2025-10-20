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
#include <chrono>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <poll.h>
#include <sstream>
#include <algorithm>

#include <unistd.h>
#include <fcntl.h>

#include "Session.hpp"
#include "Station.hpp"
#include "../network/NetworkManager.hpp"


#define SESSION_UDP_PORT 8900
#define MAX_MSG_LEN 1024

#ifndef SESSION_API_HPP
#define SESSION_API_HPP



enum SessionCommands {CREATE, JOIN, DESTROY, LEAVE, BEACON_STATION, BEACON_SESSION, DISCOVER, DISCOVER_RESPONSE, SESSION_REQUEST, SESSION_RESPONSE, STATION_PROMOTE, STATION_QUERY_VERSION, SESSION_ID_IN_USE, STATION_RESPONSE_VERSION, ECHO_REQUEST, ECHO_REPLY};

class SessionAPI {

    public:
        SessionAPI(std::shared_ptr<Config> runningConsoleConfig);
        void init(interfaceMeta interface);
        void start();
        void stop();
        void broadcastStationAndState();
        void replySessionIDInUse(struct sockaddr_in* remoteAddress, unsigned int sessionID);
        void replyStation(struct sockaddr_in* remoteAddress);
        void createSession(unsigned int sessionID, const char * sessionName);
        void discover();


        unsigned int sessionID;
    private:
        std::shared_ptr<Config> runningConsoleConfig;
        char header[6] = { 'I', 'L', 'C', 'N', 'E', 'T'};
        int socketFD;
        int opt;
        bool shouldThreadStop;
        interfaceMeta currentInterface;
        struct sockaddr_in localAddress;   
        struct sockaddr_in broadcastAddress;
        std::thread receiveThread;
        void receiveLoop();
};

#endif
