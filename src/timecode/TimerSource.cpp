#include "timecode/TimerSource.hpp"

TimerSource::TimerSource()
{
    this->state = TIMER_STOPPED;
};

TimerSourceState TimerSource::getState()
{
    return this->state;
};

void TimerSource::setState(TimerSourceState state)
{
    this->state = state;
};
