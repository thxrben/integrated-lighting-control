#include "timecode/TimerState.hpp"

#ifndef TIMERSOURCE_HPP
#define TIMERSOURCE_HPP

class TimerSource {
    protected:
        TimerSourceState state;
        void setState(TimerSourceState state);

    public:
        TimerSource();
        virtual ~TimerSource() = default;
        virtual void start() = 0;
        virtual void resume() = 0;
        virtual void pause() = 0;
        virtual void stop() = 0;
        
        TimerSourceState getState();
        virtual void freeMemory() = 0;
};

#endif