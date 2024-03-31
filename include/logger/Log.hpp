#include <iostream>
#include <cstring>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include "LogEvent.hpp"


#define LOG_EVENT_LENGTH_DEFAULT 100

#ifndef LOG_EVENT_LENGTH_MAX
#define LOG_EVENT_LENGTH_MAX 200
#endif


#ifndef LOG_HPP
#define LOG_HPP

class Log {
	private:
		LogEvent* logEvents;
		int logLength;
		int loggedEvents;
	public:
		Log();
		void createLog(int logLength);
		void logException(int errorID, int errorSource);
		void stop();
		void test();
		void writeLogToDisk();
};

#endif
