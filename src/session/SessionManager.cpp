#include "session/SessionManager.hpp"

SessionManager::SessionManager(std::shared_ptr<Config> runningConsoleConfig, std::shared_ptr<NetworkManager> networkManager) {
    this->networkManager = networkManager;
    this->runningConsoleConfig = runningConsoleConfig;
    this->sessionAPI = std::make_unique<SessionAPI>(runningConsoleConfig);
};

SessionManager::~SessionManager() {
    this->sessionAPI->stop();
    this->sessionAPI.reset();
};

void SessionManager::init() {
    this->sessionAPI->init(this->networkManager.get()->retrieveInterfaces().at(0));
    this->sessionAPI->start();
}

void SessionManager::createSession(unsigned int sessionID, char* sessionName) {
    
}

void SessionManager::leaveSession() {
    if(this->currentState != CONNECTED) {
        return;
    }

}

void SessionManager::destroySession() {
    if(this->currentState != MASTER) {
        return;
    }

}