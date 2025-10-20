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

#include "timecode/InternalTimer.hpp"

#include <libremidi/libremidi.hpp>

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"

#define ANSI_COLOR_RESET "\x1b[0m"

using namespace std;

std::shared_ptr<Config> runningConsoleConfig;

void signalHandler(int signum)
{
    printf("\n\n[" ANSI_COLOR_RED "ILC" ANSI_COLOR_RESET "] RECEIVED SIGNAL %d\n", signum);

    if (signum == SIGTERM)
    {
        printf("[\n\n" ANSI_COLOR_RED "ILC" ANSI_COLOR_RESET "] " ANSI_COLOR_RED "SIGTERM - STOPPING" ANSI_COLOR_RESET "\n\n");
        currentState = STOPPING;
    }

    if (signum == SIGINT)
    {
        printf("\n\n[" ANSI_COLOR_RED "ILC" ANSI_COLOR_RESET "] " ANSI_COLOR_YELLOW "INTERRUPT - STOPPING" ANSI_COLOR_RESET "\n\n");
        currentState = STOPPING;
    }
}

int main()
{
    std::unique_ptr<DMXEngine> engine;
    std::shared_ptr<Log> logger;
    std::unique_ptr<SessionManager> sessionManager;
    std::shared_ptr<NetworkManager> networkManager;
    std::unique_ptr<IPCManager> ipcManager;
    std::shared_ptr<Config> runningConsoleConfig;

    printf("Initalising ILC.\n\n");
    printf("ILC Process running on PID %d / TID %d.\n", getpid(), gettid());

    signal(SIGTERM, signalHandler); // Handle incoming Signals

    runningConsoleConfig = std::make_shared<Config>();

    logger = std::make_shared<Log>();

    // ipcManager = std::make_unique<IPCManager>(runningConsoleConfig, logger);
    // ipcManager->start(); // Start the IPC

    networkManager = std::make_shared<NetworkManager>(runningConsoleConfig);
    sessionManager = std::make_unique<SessionManager>(runningConsoleConfig, networkManager); // Pass networkManager for IP-Addresses

    engine = std::make_unique<DMXEngine>(); // Start the DMX-Engine

    engine->addOutput(std::make_shared<ArtNet>()); // Add ArtNet Output

    networkManager->updateIPs();

    sessionManager->init();

    engine->start();

    // The Engine has prepared for everything.
    // Now we can start the IPC:

    std::unique_ptr<InternalTimer> timer = std::make_unique<InternalTimer>();
    timer->start();

    currentState = State::RUNNING;
    // std::this_thread::sleep_for(10000ms); // Demo Timeout
    while (currentState != State::STOPPING)
    {

        if (currentState == State::STOP_REQUEST)
        {                                   // Internal console event (Stop-Request)
            currentState = State::STOPPING; // STOP PROGRAM
        }
        printf("[" ANSI_COLOR_GREEN "ILC" ANSI_COLOR_RESET "] " ANSI_COLOR_GREEN "Average FPS: %f" ANSI_COLOR_RESET "\n", engine->getAverageOutputFps());
        printf("[" ANSI_COLOR_GREEN "ILC" ANSI_COLOR_RESET "] " ANSI_COLOR_GREEN "Average Effect FPS: %f" ANSI_COLOR_RESET "\n", engine->getAverageEffectFps());
        std::this_thread::sleep_for(500ms);
    }

    timer->pause();
    long double elapsedTime = timer->getElapsedMs().count();
    long double fps = timer->getFrameCount() / elapsedTime;
    long double spf = elapsedTime / timer->getFrameCount();
    printf("[" ANSI_COLOR_GREEN "ILC" ANSI_COLOR_RESET "] " ANSI_COLOR_GREEN "Timer Stopped - Elapsed Time: %Lf ms" ANSI_COLOR_RESET "\n", elapsedTime);
    printf("[" ANSI_COLOR_GREEN "ILC" ANSI_COLOR_RESET "] " ANSI_COLOR_GREEN "Timer Stopped - Frames: %lld" ANSI_COLOR_RESET "\n", timer->getFrameCount());
    printf("[" ANSI_COLOR_GREEN "ILC" ANSI_COLOR_RESET "] " ANSI_COLOR_GREEN "Timer Paused - Time per Frame: %Lf ms" ANSI_COLOR_RESET "\n", spf);
    printf("[" ANSI_COLOR_GREEN "ILC" ANSI_COLOR_RESET "] " ANSI_COLOR_GREEN "Timer Paused - Frame per Time: %Lf ms" ANSI_COLOR_RESET "\n", fps);
    timer->stop();
    timer->freeMemory();

    // Window/App has closed
    // Therefore, we stop and free the dmxengine
    engine->stop();
    engine->freeMemory();

    // All threads have stopped. Exit:
    timer.reset();
    sessionManager.reset();
    networkManager.reset();
    engine.reset();
    logger.reset();
    runningConsoleConfig.reset();

    return 0;
};
