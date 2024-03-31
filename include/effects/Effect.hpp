#include <chrono>

#ifndef EFFECT_HPP
#define EFFECT_HPP

class Effect {
    public:
        Effect() = default;
        virtual double calcValue(std::chrono::milliseconds diffMs) = 0;
};

#endif