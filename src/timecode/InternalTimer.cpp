#include <timecode/InternalTimer.hpp>

InternalTimer::InternalTimer()
{
    this->state = TIMER_STOPPED;
};

void InternalTimer::start()
{
    if (this->state == TIMER_RUNNING)
        return;

    this->startMs = this->currentMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch());
    this->currentMs = std::chrono::milliseconds(0);
    this->state = TIMER_RUNNING;
    this->timerThread = std::thread(&InternalTimer::run, this);
};

void InternalTimer::resume()
{
    if (this->state != TIMER_PAUSED)
        return;

    this->state = TIMER_RUNNING;
};

void InternalTimer::pause()
{
    if (this->state != TIMER_RUNNING)
        return;

    this->state = TIMER_PAUSED;
};

void InternalTimer::stop()
{
    this->startMs = std::chrono::milliseconds(0);
    this->currentMs = std::chrono::milliseconds(0);
    this->state = TIMER_STOPPED;
    this->timerThread.join();
};

void InternalTimer::run()
{
    FrameLimit<42> frameLimiter;
    std::chrono::milliseconds currentMs;

    while (this->state == TIMER_RUNNING) {
        
        this->currentMs = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch());

        this->elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(this->currentMs - this->startMs);
        this->frameCount = (long long) (this->elapsedMs.count() / 84);

        frameLimiter.sleep();
    }
};

void InternalTimer::freeMemory()
{

}