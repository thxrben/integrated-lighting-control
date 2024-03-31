#include "LogEvent.hpp"

/*
    #   Just a single LogEvent.
    #   Takes the errorID and the errorSource.
*/

LogEvent::LogEvent(int errorID, int errorSource) {
    this->errorID = errorID;
    this->errorSource = errorSource;
};

int LogEvent::getErrorID() {
    return this->errorID;
};

int LogEvent::getErrorSource() {
    return this->errorSource;
};

void LogEvent::stop() {
    // if(errorDescription != nullptr) {
    //     free(errorDescription);
    //     this->errorDescription = nullptr;
    // }
}