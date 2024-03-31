#include "effects/SineWave.hpp"

SineWave::SineWave() : Effect() {

};

double SineWave::calcValue(std::chrono::milliseconds diffMs) {
    return std::sin((float) (5)*((2*M_PI)/ 50 * (diffMs.count())));
};