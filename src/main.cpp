#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include <csignal>
#include <memory>

#include "config/Config.hpp"
#include "ipc/IPCManager.hpp"

#include "logger/Log.hpp"

#include "state/State.hpp"

#include "dmx_engine/DMXEngine.hpp"
#include "dmx_output/ArtNet.hpp"

#include "session/SessionManager.hpp"
#include "network/NetworkManager.hpp"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"

#define ANSI_COLOR_RESET   "\x1b[0m"

using namespace std;

void signalHandler( int signum ) {
    printf("\n\n[" ANSI_COLOR_RED "ILC" ANSI_COLOR_RESET "] RECEIVED SIGNAL %d\n", signum);
    
    if(signum == SIGTERM) {
        printf("[\n\n" ANSI_COLOR_RED "ILC" ANSI_COLOR_RESET "] " ANSI_COLOR_RED "SIGTERM - STOPPING" ANSI_COLOR_RESET "\n\n");
        currentState = STOPPING;
    }

    if(signum == SIGINT) {
        printf("\n\n[" ANSI_COLOR_RED "ILC" ANSI_COLOR_RESET "] " ANSI_COLOR_YELLOW "INTERRUPT - STOPPING" ANSI_COLOR_RESET "\n\n");
        currentState = STOPPING;
    }
}

int main() {
    std::unique_ptr<DMXEngine> engine; 
    std::shared_ptr<Log> logger;
    std::unique_ptr<SessionManager> sessionManager;
    std::shared_ptr<NetworkManager> networkManager;
    std::unique_ptr<IPCManager> ipcManager;

    printf("Initalising ILC.\n\n");
    printf("ILC Process running on PID %d / TID %d.\n", getpid(), gettid());

    readConfig();

    signal(SIGTERM, signalHandler); // Handle incoming Signals

    logger = std::make_shared<Log>();

    ipcManager = std::make_unique<IPCManager>(logger);
    ipcManager->start(); // Start the IPC

    networkManager = std::make_shared<NetworkManager>();
    sessionManager = std::make_unique<SessionManager>(networkManager); // Pass networkManager for IP-Addresses

    engine = std::make_unique<DMXEngine>(); // Start the DMX-Engine
    
    networkManager->updateIPs();
    
    sessionManager->init();

    //engine->addOutput(new ArtNet());
    engine->start();

    // The Engine has prepared for everything.
    // Now we can start the IPC:

    currentState = RUNNING;
    // std::this_thread::sleep_for(10000ms); // Demo Timeout
    while(currentState != STOPPING) {

        if(currentState == STOP_REQUEST) { // Internal console event (Stop-Request)
           currentState == STOPPING; // STOP PROGRAM
        }

        std::this_thread::sleep_for(500ms);
    }

    // Window/App has closed
    // Therefore, we stop and free the dmxengine 
    engine->stop();
    engine->freeMemory();


    // All threads have stopped. Exit:
    sessionManager.reset();
    networkManager.reset();
    engine.reset();
    logger.reset();

    return 0;
};
