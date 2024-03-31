#include <stdio.h>
#include <stdlib.h>
#include <string>

#ifndef LOG_EVENT_HPP
#define LOG_EVENT_HPP

class LogEvent {
    private:
        int errorID;
        int errorSource;
        std::string errorDescription;
    public:
        LogEvent(int errorID, int errorSource);
        int getErrorID();
        int getErrorSource();
        void stop();
};

#endif