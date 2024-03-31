
#include <chrono>
#include <cmath>

#include "Effect.hpp"

#ifndef SineWave_HPP
#define SineWave_HPP

class SineWave : public Effect {
    public:
        SineWave();
        double calcValue(std::chrono::milliseconds diffMs) ;
};


#endif