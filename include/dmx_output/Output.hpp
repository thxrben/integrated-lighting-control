#include <memory> 
#include "../dmx_engine/Universe.hpp"

#ifndef DMX_OUTPUT_HPP
#define DMX_OUTPUT_HPP

class Output {
    private:
    public:
        Output();
        virtual void sendUniverse(std::shared_ptr<Universe> universe, unsigned char sequenceNum) = 0;
        virtual void stop() = 0;
        virtual void freeMemory() = 0;
        virtual ~Output() = default;
};

#endif