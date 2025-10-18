#include <chrono>

#include <time.h>
#include <thread>

#include "TimerSource.hpp"
#include "timecode/TimerState.hpp"
#include "dmx_engine/FrameLimit.hpp"


#ifndef INTERNAL_TIMER_HPP
#define INTERNAL_TIMER_HPP



class InternalTimer : public TimerSource {
    private:
        std::chrono::milliseconds startMs;
        std::chrono::milliseconds currentMs;
        std::chrono::milliseconds elapsedMs;
        long long int frameCount = 0;
        std::thread timerThread;
        void run();
    public:
        InternalTimer();
        void start();
        void resume();
        void pause();
        void stop();
        TimerSourceState getState();
        std::chrono::milliseconds getElapsedMs() { return this->elapsedMs; }
        std::chrono::milliseconds getStartMs() { return this->startMs; }
        long long int getFrameCount() { return this->frameCount; }
        void freeMemory();
};

#endif