#include "SessionAPI.hpp"

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"

#define ANSI_COLOR_RESET "\x1b[0m"

using namespace std;

SessionAPI::SessionAPI()
{
    this->shouldThreadStop = false;
};

void SessionAPI::init(interfaceMeta interface)
{
    int broadcastEnable = 1;
    int sessionID = currentConfig.get<int>("session.sessionID");
    int socketFlags;

    std::string sessionNameStr = currentConfig.get<std::string>("session.sessionName");
    const char *sessionName = sessionNameStr.c_str();

    this->currentInterface = interface;

    this->socketFD = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (this->socketFD < 0)
    {
        perror("[SessionAPI] Could not create UDP-Socket for SessionAPI Output!");
        exit(1);
    }

    if (setsockopt(this->socketFD, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) < 0)
    {
        printf("[SessionAPI] Socket Options failed!\n");
        exit(2);
    }

    /* set up local address and port of the socket*/
    memset(&localAddress, 0, sizeof(localAddress));
    localAddress.sin_family = AF_INET;
    localAddress.sin_port = htons(SESSION_UDP_PORT); // Host to Network Byte Order
    localAddress.sin_addr = interface.ipAddr;

    /* set up broadcast address */
    memset(&broadcastAddress, 0, sizeof(broadcastAddress));
    broadcastAddress.sin_family = AF_INET;
    broadcastAddress.sin_port = htons(SESSION_UDP_PORT); // Host to Network Byte Order
    broadcastAddress.sin_addr = interface.broadcast;

    if (bind(this->socketFD, (struct sockaddr *)&broadcastAddress, sizeof(broadcastAddress)) < 0)
    {
        perror("[SessionAPI] Error binding socket");
        exit(5);
    }

    discover();

    createSession(sessionID, sessionName);
    broadcastStationAndState();

    printf(ANSI_COLOR_RESET "[" ANSI_COLOR_MAGENTA "SessionAPI" ANSI_COLOR_RESET "] : ILCNET started on Interface with IP %s.\n", inet_ntoa(interface.ipAddr));
};

void SessionAPI::start()
{
    this->receiveThread = std::thread(&SessionAPI::receiveLoop, this);
}

void SessionAPI::receiveLoop()
{
    char buffer[MAX_MSG_LEN];

    struct sockaddr_in remoteAddr;
    socklen_t addrLen = sizeof(remoteAddr);

    printf(ANSI_COLOR_RESET "[" ANSI_COLOR_MAGENTA "SessionAPI" ANSI_COLOR_RESET "] : Receiveloop started on PID %d / TID %d\n", getpid(), gettid());

    while (!this->shouldThreadStop)
    {
 
        int bytesReceived = recvfrom(this->socketFD, buffer, MAX_MSG_LEN, 0, (struct sockaddr *)&remoteAddr, &addrLen);

        if (bytesReceived <= 0)
        {
            if(errno == 0)
                return;

            std::cerr << "Error receiving ILCNET-Message (" << bytesReceived << ") - " << strerror(errno) << " - " << errno << std::endl;
            continue;
        }

        printf("RECEIVED %d BYTES\n\n", bytesReceived);

        in_addr remoteIp = remoteAddr.sin_addr;
        in_addr localIP = this->currentInterface.ipAddr;

        printf("Packet from (%s). I am at (%s)\n", inet_ntoa(remoteIp), inet_ntoa(localIP));

        if (remoteIp.s_addr == localIP.s_addr)
        {
            // Sender-Address is the same as this local device's. Discard!
            printf("Received my own packet (%s). Yikes!\n", inet_ntoa(remoteIp));
            continue;
        }

        //TODO: Check for ILCNET-Header

        switch (buffer[6]) // buffer[6] is the INTENT of the ILCNET message
        {
        case CREATE:
            printf("Someone created a Session! (ID: %d) (Length: %d)\n", buffer[7], bytesReceived);
            if (buffer[7] == this->sessionID)
            {
                printf("[SessionAPI] Duplicate SessionID found!!!\n");
                this->replySessionIDInUse(&remoteAddr, buffer[7]);
            }
            // Add the session to our local list
            // Check here if this station is operating with the ID of the new Session. If yes, notify the the other station!
            break;
        case JOIN:
            printf("Someone wants to join this Session!\n");
            break;
        case DESTROY:
            printf("The Session you participated in has been destroyed!\n");
            // Notify components to stop operating in session mode
            // Go into single operation mode
            break;
        case LEAVE:
            printf("A station has left the session!");
            // Remove station from session list
            break;
        case BEACON_STATION:
            printf("Station was found (BEACON_STATION received)!...\n");
            this->availableStations.push_back(std::make_unique<Station>(remoteIp));
            break;
        case BEACON_SESSION:
            printf("Session was found! (ID: %d) (Length: %d)\n", buffer[7], bytesReceived);
            if (buffer[7] == this->sessionID)
            {
                printf("[SessionAPI] Duplicate SessionID found!!!\n");
                this->replySessionIDInUse(&remoteAddr, buffer[7]);
            }
            break;
        case DISCOVER:
            printf("Replying to DISCOVER Request...\n");
            replyStation(&remoteAddr);
            break;
        case DISCOVER_RESPONSE:
            printf("Someone has replied to this stations discover request!\n");
            break;
        case SESSION_REQUEST:
            printf("Someone has asked for sessions!\n");
            break;
        case SESSION_RESPONSE:
            printf("Someone has replied to this stations session request!\n");
            break;
        case STATION_PROMOTE:
            printf("A Stations which acted as a session master promotes another console to master instead!\n");
            break;
        case STATION_QUERY_VERSION:
            printf("Someone queried this stations software version!");
            break;
        default:
            break;
        }

    }
    
    printf(ANSI_COLOR_RESET "[" ANSI_COLOR_MAGENTA "SessionAPI" ANSI_COLOR_RESET "] : Receiveloop ended.\n");
};


void sendPacket(int socketFD, char* dataArray, int dataArrayLength, struct sockaddr_in *remoteAddress) {

    printf("SocketFD %d sending to %s\n", inet_ntoa(remoteAddress->sin_addr));
    int res = sendto(socketFD, dataArray, dataArrayLength, 0, (struct sockaddr *) remoteAddress, sizeof((*remoteAddress)));

    if (res < 0)
    {
        printf("ReplyStation: RES: %d\n", res);
        perror("sendto");
        exit(1);
    }
    

}

void SessionAPI::replyStation(struct sockaddr_in *remoteAddress)
{
    int length = 7;

    char *finalArray = (char *)malloc(length);

    *(finalArray + 0) = 'I';
    *(finalArray + 1) = 'L';
    *(finalArray + 2) = 'C';
    *(finalArray + 3) = 'N';
    *(finalArray + 4) = 'E';
    *(finalArray + 5) = 'T';
    *(finalArray + 6) = BEACON_STATION;

    sendPacket(this->socketFD, finalArray, length, remoteAddress);
    
    if (finalArray == nullptr)
        return;

    free(finalArray);
    finalArray = nullptr;
}

void SessionAPI::discover()
{
    char *finalArray = (char *)malloc(7);

    *(finalArray + 0) = 'I';
    *(finalArray + 1) = 'L';
    *(finalArray + 2) = 'C';
    *(finalArray + 3) = 'N';
    *(finalArray + 4) = 'E';
    *(finalArray + 5) = 'T';
    *(finalArray + 6) = DISCOVER;

    int res = sendto(this->socketFD, finalArray, 7, 0, (struct sockaddr *)&(this->broadcastAddress), sizeof(this->broadcastAddress));
    if (res < 0)
    {
        printf("RES: %d\n", res);
        perror("sendto");
        exit(1);
    }

    if (finalArray == nullptr)
        return;

    free(finalArray);
    finalArray = nullptr;
}

void SessionAPI::replySessionIDInUse(struct sockaddr_in *remoteAddress, unsigned int sessionID)
{
    int length = 8;

    char *finalArray = (char *)malloc(length);

    *(finalArray + 0) = 'I';
    *(finalArray + 1) = 'L';
    *(finalArray + 2) = 'C';
    *(finalArray + 3) = 'N';
    *(finalArray + 4) = 'E';
    *(finalArray + 5) = 'T';
    *(finalArray + 6) = SESSION_ID_IN_USE;
    *(finalArray + 7) = sessionID;

    int res = sendto(this->socketFD, finalArray, length, 0, (struct sockaddr *)remoteAddress, sizeof((*remoteAddress)));

    if (res < 0)
    {
        printf("DuplicateSessionMSG: RES: %d\n", res);
        perror("sendto");
        exit(1);
    }
    // free(sessionName);
    if (finalArray == nullptr)
        return;

    free(finalArray);
    finalArray = nullptr;
}

void SessionAPI::createSession(unsigned int sessionID, const char *sessionName)
{
    unsigned char sessionNameLength = std::min((int)strlen(sessionName), 255);

    int length = sessionNameLength + 9;

    char *finalArray = (char *)malloc(length);

    *(finalArray + 0) = 'I';
    *(finalArray + 1) = 'L';
    *(finalArray + 2) = 'C';
    *(finalArray + 3) = 'N';
    *(finalArray + 4) = 'E';
    *(finalArray + 5) = 'T';
    *(finalArray + 6) = CREATE;
    *(finalArray + 7) = sessionID;
    *(finalArray + 8) = sessionNameLength;

    for (int i = 0; i < sessionNameLength; i++)
    {
        *(finalArray + i + 9) = *(sessionName + i);
    }

    this->sessionID = sessionID;

    int res = sendto(this->socketFD, finalArray, length, 0, (struct sockaddr *)&(this->broadcastAddress), sizeof(this->broadcastAddress));

    if (res < 0)
    {
        printf("RES: %d\n", res);
        perror("sendto");
        exit(1);
    }
    // free(sessionName);
    if (finalArray == nullptr)
        return;

    free(finalArray);
    finalArray = nullptr;
}

void SessionAPI::broadcastStationAndState()
{
    char *finalArray = (char *)malloc(8);

    *(finalArray + 0) = 'I';
    *(finalArray + 1) = 'L';
    *(finalArray + 2) = 'C';
    *(finalArray + 3) = 'N';
    *(finalArray + 4) = 'E';
    *(finalArray + 5) = 'T';
    *(finalArray + 6) = BEACON_SESSION;
    *(finalArray + 7) = this->sessionID;

    int res = sendto(this->socketFD, finalArray, sizeof(finalArray), 0, (struct sockaddr *)&(this->broadcastAddress), sizeof(this->broadcastAddress));

    if (res < 0)
    {
        printf(ANSI_COLOR_RESET "[" ANSI_COLOR_MAGENTA "SessionAPI" ANSI_COLOR_RESET "] (StationBeacon) Could not send: Result: %d\n", res);
        perror("sendto");
        // exit(1);
    }

    if (finalArray == nullptr)
        return;

    free(finalArray);
    finalArray = nullptr;
}

void SessionAPI::stop()
{
    this->shouldThreadStop = true;

    shutdown(this->socketFD, SHUT_RDWR);

    receiveThread.join();
    printf("[SESSIONAPI] Done.");
    close(this->socketFD);
    
};
