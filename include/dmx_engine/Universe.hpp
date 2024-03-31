#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../fixture/Fixture.hpp"

#ifndef UNIVERSE_HPP
#define UNIVERSE_HPP

class Universe {
    private:
        unsigned char* dmxChannels;
        unsigned short universe;
        std::vector<Fixture*> fixtures;

    public:
        Universe(unsigned short universe);
        void freeMemory();
        void refreshUniverseData();
        unsigned short getUniverseNum();
        unsigned char* getDMXValues();
        void setDmxValue(unsigned short channel, unsigned char value);
};

#endif