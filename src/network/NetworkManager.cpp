#include "network/NetworkManager.hpp"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"

#define ANSI_COLOR_RESET   "\x1b[0m"

static void set_if_down(char * ifname)
{
    int sockfd;
    struct ifreq ifr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0)
        return;

    memset(&ifr, 0, sizeof ifr);

    strncpy(ifr.ifr_name, ifname, strnlen(ifname, IF_NAMESIZE));

    ifr.ifr_flags =  ifr.ifr_flags & ~(IFF_UP);
    ioctl(sockfd, SIOCSIFFLAGS, &ifr);

    printf("[" ANSI_COLOR_GREEN "NetworkManager" ANSI_COLOR_RESET"] : Link " ANSI_COLOR_RED "down" ANSI_COLOR_RESET " on interface %s\n", ifname);
}

static void set_if_up(const char *ifname)
{
    int sockfd;
    struct ifreq ifr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0)
        return;

    memset(&ifr, 0, sizeof ifr);

    strncpy(ifr.ifr_name, ifname, strnlen(ifname, IF_NAMESIZE));

    ifr.ifr_flags |= IFF_UP;
    ioctl(sockfd, SIOCSIFFLAGS, &ifr);

    printf("[" ANSI_COLOR_GREEN "NetworkManager" ANSI_COLOR_RESET"] : Link " ANSI_COLOR_GREEN "up" ANSI_COLOR_RESET " on interface %s\n", ifname);
}

// Set the IP address of the specified interface to the specified address.
// Returns 0 on success, -1 on error.
int NetworkManager::set_interface_address(const char* interface, const char* address, const char* netmask)
{
    printf("[" ANSI_COLOR_GREEN "NetworkManager" ANSI_COLOR_RESET"] : Configuring interface %s to IP %s / %s.\n", interface, address, netmask);

    // Create a socket to use with the ioctl function.
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        // Failed to create socket.
        return -1;
    }

    // Set up the ifreq structure.
    ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, interface, IFNAMSIZ);

    // Set up the sockaddr_in structure for the IP address.
    sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    if (inet_aton(address, &sa.sin_addr) == 0) {
        // Failed to convert address to binary form.
        close(sock);
        return -1;
    }

    // Set up the sockaddr_in structure for the netmask.
    sockaddr_in sa_netmask;
    memset(&sa_netmask, 0, sizeof(sa_netmask));
    sa_netmask.sin_family = AF_INET;
    if (inet_aton(netmask, &sa_netmask.sin_addr) == 0) {
        // Failed to convert netmask to binary form.
        close(sock);
        return -1;
    }

    // Set the IP address and netmask of the interface.
    ifr.ifr_addr = *reinterpret_cast<sockaddr*>(&sa);
    if (ioctl(sock, SIOCSIFADDR, &ifr) < 0) {
        // Failed to set IP address.
        close(sock);
        return -1;
    }

    ifr.ifr_addr = *reinterpret_cast<sockaddr*>(&sa_netmask);
    if (ioctl(sock, SIOCSIFNETMASK, &ifr) < 0) {
        // Failed to set netmask.
        close(sock);
        return -1;
    }

    // Close the socket and return success.
    close(sock);
    return 0;
}


NetworkManager::NetworkManager(std::shared_ptr<Config> runningConsoleConfig) {
    this->runningConsoleConfig = runningConsoleConfig;
    
    printf("[" ANSI_COLOR_GREEN "NetworkManager" ANSI_COLOR_RESET"] : Setting up links...\n");
    std::string ifaceNameStr = runningConsoleConfig->getProperty<std::string>(NETWORK_IFACE_NAME);
    std::string eth0AddrStr = runningConsoleConfig->getProperty<std::string>(NETWORK_ETH_ADDR);
    std::string eth0MaskStr = runningConsoleConfig->getProperty<std::string>(NETWORK_ETH_MASK);
    const char * ifaceName = ifaceNameStr.c_str();
    const char * eth0Addr = eth0AddrStr.c_str();
    const char * eth0Mask = eth0MaskStr.c_str();
    set_interface_address(ifaceName, eth0Addr, eth0Mask);

    set_if_up(ifaceName);
    
    printf("[" ANSI_COLOR_GREEN "NetworkManager" ANSI_COLOR_RESET"] : Init done.\n");
};

NetworkManager::~NetworkManager() {
    //set_if_down("eth0"); // TODO: Production: Uncomment!!
    this->interfaces.clear();
    
    printf("[" ANSI_COLOR_GREEN "NetworkManager" ANSI_COLOR_RESET"] : Networkmanager done.\n");
}

void NetworkManager::updateIPs() {
    struct ifaddrs *addrs,*tmp;
    struct sockaddr_in *sa;
    struct sockaddr_in *netmask;


    getifaddrs(&addrs);
    tmp = addrs;
    
    printf("[" ANSI_COLOR_GREEN "NetworkManager" ANSI_COLOR_RESET"] : Available interfaces:\n");
    
    while (tmp)
    {
        if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET) {
            printf("IFACE: %s\n", tmp->ifa_name);
            
            if(strcmp(tmp->ifa_name, "lo") != 0) {
                sa = (struct sockaddr_in *) tmp->ifa_addr;
                printf("|---ADDRESS:   %s \n", inet_ntoa(sa->sin_addr));
                
                netmask = (struct sockaddr_in *) tmp->ifa_netmask;

                if(netmask != NULL) {

                    interfaceMeta meta;

                    printf("|---NETMASK:   %s \n", inet_ntoa(netmask->sin_addr));

                    struct in_addr broad; 
                    broad.s_addr = (sa->sin_addr).s_addr | (~((netmask->sin_addr).s_addr));

                    printf("|---BROADCAST: %s\n", inet_ntoa(broad));
                    //broadcast = ip | ( ~ subnet )

                    meta.ipAddr = sa->sin_addr;
                    meta.netmask = netmask->sin_addr;
                    meta.broadcast = broad;

                    this->interfaces.push_back(meta);
                }
                
            }
            printf("\n");
        }
        tmp = tmp->ifa_next;
    }
    freeifaddrs(addrs);
};

std::vector<interfaceMeta> NetworkManager::retrieveInterfaces() {
    return this->interfaces;
}
