#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <thread>


#ifndef FRAMELIMIT_HPP
#define FRAMELIMIT_HPP

template<std::intmax_t FPS>
class FrameLimit {
public:
    FrameLimit() :                 // initialize the object keeping the pace
        time_between_frames{1},     // std::ratio<1, FPS> seconds
        tp{std::chrono::steady_clock::now()}
    {}

    void sleep();

private:
    // a duration with a length of 1/FPS seconds
    std::chrono::duration<double, std::ratio<1, FPS>> time_between_frames;

    // the time point we'll add to in every loop
    std::chrono::time_point<std::chrono::steady_clock, decltype(time_between_frames)> tp;
};

template<std::intmax_t FPS>
void FrameLimit<FPS>::sleep() {
        // add to time point
        this->tp += time_between_frames;

        // and sleep until that time point
        std::this_thread::sleep_until(tp);
};

#endif