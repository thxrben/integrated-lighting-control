#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>

#include <zmqpp/zmqpp.hpp>

#include "../logger/Log.hpp"
#include "../config/Config.hpp"

#ifndef IPCMANAGER_HPP
#define IPCMANAGER_HPP

class IPCManager {
    private: 
        const std::string endpoint = "tcp://127.0.0.1:5555";
        bool shouldThreadStop;
        std::shared_ptr<Log> logger;
        std::shared_ptr<zmqpp::context> context;
        zmqpp::socket_type type; // No PTR needed, just an enum
        std::shared_ptr<zmqpp::socket> socket;
        std::thread zmqppThread;
        std::shared_ptr<Config> runningConsoleConfig;
    public:
        IPCManager(std::shared_ptr<Config> runningConsoleConfig, std::shared_ptr<Log> logger);
        void start();
        void receiveLoop();
        ~IPCManager();

};

#endif