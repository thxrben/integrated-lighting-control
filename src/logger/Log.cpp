#include "Log.hpp"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


/*
    # This Log can hold up multiple LogEvents.
    # If it triggers an exception, it will create a new one and exit immidiately.
    # Will be able to print out the Log to a file
*/

Log::Log() {
    this->logLength = 0;
};

void Log::createLog(int logLength) {

    if(logLength >= LOG_EVENT_LENGTH_MAX) {
        printf("[LOG] Could not create log with a size of %d. Creating one with %d now.", logLength, LOG_EVENT_LENGTH_DEFAULT);
        logEvents = (LogEvent*) malloc(sizeof(LogEvent) * LOG_EVENT_LENGTH_DEFAULT);
    } else {
        logEvents = (LogEvent*) malloc(sizeof(LogEvent) * logLength);
    }

    if(logEvents != NULL) {
        this->logLength = logLength; // Shows that the log has been created!
    }
};

void Log::test() {
    printf("LOG TEST \n");
}

void Log::logException(int errorID, int errorSource) {
    printf(ANSI_COLOR_RED "[" ANSI_COLOR_YELLOW "LOG" ANSI_COLOR_RED "] (" ANSI_COLOR_CYAN "%d" ANSI_COLOR_RED") : A EXCEPTION WITH STATUS " ANSI_COLOR_CYAN "%d" ANSI_COLOR_RED " OCCURED. \n" ANSI_COLOR_RESET, errorSource, errorID);
    this->writeLogToDisk(); // Save Log Entries
    exit(errorID);
};

void Log::stop() {
    if(logLength > 0 && logEvents != NULL) {
        
        for(int i = 0; i < loggedEvents; i++) {
            LogEvent* currentLog = (logEvents + (sizeof(LogEvent) * i));
            currentLog->stop();
        }
        
        if(logEvents == nullptr)
            return;

        free(logEvents);
        logEvents = nullptr;
    }
}

void Log::writeLogToDisk() {
    return;
    // int fdLog = open("error.log", O_RDWR | O_CREAT);
    // char *errorMsg = "AN EXCEPTION OCCURED. ERROR REPORT BELOW:\n";
    // write(fdLog, errorMsg, strlen(errorMsg));
    
    // for(int i = 0; i < loggedEvents; i++) {
    //     LogEvent* currentLog = (logEvents + (sizeof(LogEvent) * i));
    //     int errorID = currentLog->getErrorID();
    //     int errorSource = currentLog->getErrorSource();
    //     char s[128] = {};
    //     sprintf(s, "ERR_ID: %d | ERR_SOURCE: %d\n", errorID, errorSource);
    //     write(fdLog, s, strlen(s));
    //     printf(s);
    // }

    // close(fdLog);
}